#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.hpp"

#include "../ApprovalTests/namers/ApprovalTestNamer.h"

using namespace doctest;

struct DocTestReporterStub : IReporter
{
    DocTestReporterStub()
    {
    }

    void test_case_start(const TestCaseData& /*testInfo*/) override
    {
    }

    void test_case_end(const CurrentTestCaseStats& /*in*/) override
    {
    }

    void report_query(const QueryData &/*data*/) override
    {
    }

    void test_run_start() override
    {
    }

    void test_run_end(const TestRunStats &/*stats*/) override
    {
    }

    void test_case_exception(const TestCaseException &/*exception*/) override
    {
    }

    void subcase_start(const SubcaseSignature &/*signature*/) override
    {

    }

    void subcase_end() override
    {
    }

    void log_assert(const AssertData &/*data*/) override
    {
    }

    void log_message(const MessageData &/*data*/) override
    {
    }

    void test_case_skipped(const TestCaseData &/*data*/) override
    {
    }
};

struct DocTestApprovalListener : DocTestReporterStub
{
//    using TestEventListenerBase::TestEventListenerBase;
    TestName currentTest;

    // constructor has to accept the ContextOptions by ref as a single argument
    DocTestApprovalListener(const ContextOptions& /*in*/)
    {
    }
    
    void test_case_start(const TestCaseData& testInfo) override
    {
        currentTest.setFileName(testInfo.m_file);
        ApprovalTestNamer::currentTest(&currentTest);
    }

    void test_case_end(const CurrentTestCaseStats& /*in*/) override
    {
        while (!currentTest.sections.empty()) {
            currentTest.sections.pop_back();
        }
    }

    void subcase_start(const SubcaseSignature &signature) override
    {
        currentTest.sections.push_back(signature.m_name);
    }

    void subcase_end() override
    {
        currentTest.sections.pop_back();
    }
};

REGISTER_REPORTER("approvals", 0, DocTestApprovalListener);

int main(int argc, char** argv)
{
    const char *myargv[] = {"program name", "--reporters=approvals", NULL};
    int myargc = sizeof(myargv) / sizeof(char*) - 1;

    return doctest::Context(myargc, myargv).run();
}
