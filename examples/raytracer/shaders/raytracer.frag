#version 450 core

// Simple ray tracer example
// Based on blog post originally from Three-Eyed Games:
// https://www.gamedeveloper.com/programming/gpu-ray-tracing-in-unity-part-1

in vec2 TexCoord;

uniform ivec2 resolution;

uniform mat4 inverseView;
uniform mat4 inverseProjection;
uniform vec3 cameraPosition;

uniform vec3 lightDirection;
uniform float lightIntensity;

uniform vec3 spherePositions[64];
uniform float sphereSizes[64];
uniform vec3 sphereAlbedo[64];
uniform vec3 sphereSpecular[64];
uniform int sphereCount;

uniform vec3 planeColor;
uniform vec3 planeSpecular;

uniform samplerCube skybox;

uniform uint MSAA_FACTOR;
uniform uint MAX_BOUNCES;
uniform uint SPHERE_LIMIT;

uniform bool RENDER_SHADOWS;

uniform float INF = 3.4028233466e38;

layout(location = 0) out vec4 FragColor;

struct Ray {
    vec3 origin;
    vec3 direction;
    vec3 energy;
};

Ray createRay(vec3 origin, vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    ray.energy = vec3(1.0f);

    return ray;
}

struct RayHit {
    vec3 position;
    float distance;
    vec3 normal;
    vec3 albedo;
    vec3 specular;
};

RayHit createRayHit() {
    RayHit hit;
    hit.position = vec3(0.0f);
    hit.distance = INF;
    hit.normal = vec3(0.0f);

    return hit;
}

Ray createCameraRay(vec2 uv) {
    vec3 direction = (inverseProjection * vec4(uv, 0.0f, 1.0f)).xyz;
    direction = (inverseView * vec4(direction, 0.0f)).xyz;
    direction = normalize(direction);

    Ray ray = createRay(cameraPosition, direction);
    return ray;
}

void intersectGroundPlane(Ray ray, inout RayHit bestHit) {
    float t = -ray.origin.y / ray.direction.y;
    if (t > 0 && t < bestHit.distance) {
        bestHit.distance = t;
        bestHit.position = ray.origin + t * ray.direction;
        bestHit.normal = vec3(0.0f, 1.0f, 0.0f);
        bestHit.albedo = planeColor;
        bestHit.specular = planeSpecular;
    }
}

void intersectSphere(Ray ray, inout RayHit bestHit, int sphereIndex) {
    vec4 sphere = vec4(spherePositions[sphereIndex], sphereSizes[sphereIndex]);

    vec3 d = ray.origin - sphere.xyz;
    float p1 = -dot(ray.direction, d);
    float p2sqr = p1 * p1 - dot(d, d) + sphere.w * sphere.w;
    if (p2sqr < 0) return;

    float p2 = sqrt(p2sqr);
    float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;
    if (t > 0 && t < bestHit.distance) {
        bestHit.distance = t;
        bestHit.position = ray.origin + t * ray.direction;
        bestHit.normal = normalize(bestHit.position - sphere.xyz);
        bestHit.albedo = sphereAlbedo[sphereIndex];
        bestHit.specular = sphereSpecular[sphereIndex];
    }
}

RayHit trace(Ray ray) {
    RayHit bestHit = createRayHit();
    intersectGroundPlane(ray, bestHit);

    for (int i = 0; i < min(sphereCount, SPHERE_LIMIT); i++) {
        intersectSphere(ray, bestHit, i);
    }

    return bestHit;
}

vec3 shade(inout Ray ray, RayHit hit) {
    if (hit.distance < INF) {
        ray.origin = hit.position + hit.normal * 0.001f;
        ray.direction = reflect(ray.direction, hit.normal);
        ray.energy *= hit.specular;

        if (RENDER_SHADOWS) {
            bool shadow = false;
            Ray shadowRay = createRay(hit.position + hit.normal * 0.001f,
                -1 * normalize(lightDirection));
            RayHit shadowHit = trace(shadowRay);
            if (shadowHit.distance != INF) {
                return vec3(0.0f);
            }
        }

        return clamp(dot(hit.normal, lightDirection) * -1, 0.0f, 1.0f)
            * lightIntensity * hit.albedo;
    } else {
        ray.energy = vec3(0.0f);
        return texture(skybox, ray.direction).rgb;
    }
}

void main()
{
    vec2 uv = 2*TexCoord - 1.0f;
    vec2 offsetSize = 2.0f / resolution;

    vec3 averagedSum = vec3(0.0f);
    for (int x_iter = 0; x_iter < MSAA_FACTOR; x_iter++) {
        for (int y_iter = 0; y_iter < MSAA_FACTOR; y_iter++) {
            vec2 pixelOffset = vec2(x_iter, y_iter);
            pixelOffset -= (MSAA_FACTOR - 1) / 2.0f;
            pixelOffset /= MSAA_FACTOR;

            vec2 offset = pixelOffset * offsetSize;
            Ray ray = createCameraRay(uv + offset);

            vec3 result = vec3(0.0f);
            for (uint i = 0; i < MAX_BOUNCES; i++) {

                RayHit hit = trace(ray);
                vec3 energy = ray.energy;
                result += energy * shade(ray, hit);

                if (all(equal(ray.energy, vec3(0.0f)))) break;
            }

            averagedSum += result / (MSAA_FACTOR * MSAA_FACTOR);
        }
    }

    FragColor = vec4(averagedSum, 1.0);
}