#ifndef TST_APP_APPTESTUTILS_HPP_
#define TST_APP_APPTESTUTILS_HPP_

#include "App/IBasilWidget.hpp"
#include "Data/PubSubTestUtils.hpp"
#include "Process/ProcessTestUtils.hpp"

namespace basil {

class TestWidget :  public TestProcess,
                    public IDataPublisher,
                    public IBasilWidget {
 public:
    TestWidget() = default;
    void onLoop() override { TestProcess::onLoop(); }
};

}  // namespace basil

#endif  // TST_APP_APPTESTUTILS_HPP_
