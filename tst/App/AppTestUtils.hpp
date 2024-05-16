#ifndef TST_APP_APPTESTUTILS_HPP_
#define TST_APP_APPTESTUTILS_HPP_

#include "App/IBasilWidget.hpp"
#include "PubSub/PubSubTestUtils.hpp"
#include "Process/ProcessTestUtils.hpp"

namespace basil {

class TestWidget :  public TestProcess,
                    public IBasilWidget {
 public:
    TestWidget() : IBasilWidget({
        "TestWidget",
        ProcessOrdinal::MAIN,
        ProcessPrivilege::NONE,
        WidgetPubSubPrefs::ALL
    }) {}
    void onLoop() override { TestProcess::onLoop(); }
};

}  // namespace basil

#endif  // TST_APP_APPTESTUTILS_HPP_
