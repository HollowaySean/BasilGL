#include "Logging.hpp"

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    Logger& logger = Logger::get();

    logger.log("test", Level::WARN);

    return 0;
}
