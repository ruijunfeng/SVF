#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "WPA/WPAPass.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "SVFIR/SVFFileSystem.h"

using namespace llvm;
using namespace std;
using namespace SVF;

// int main() {
//     auto moduleNameVec =
//         OptionBase::parseOptions(argc, argv, "Whole Program Points-to Analysis",
//                                  "[options] <input-bitcode...>");
//     // 创建 SVF 模块
//     SVFModule svfModule = SVFModule::pagReadFromTxt("path_to_your_program");

//     // 创建 Andersen 分析对象
//     Andersen* andersen = AndersenWaveDiff::createAndersenWaveDiff(&svfModule);

//     // 获取分析后的图
//     PAG* pag = andersen->getPAG();

//     // 你现在可以使用 pag 对象来访问和操作分析后的图
//     // 例如，你可以遍历所有的节点：
//     for (PAG::iterator it = pag->begin(), eit = pag->end(); it != eit; ++it) {
//         PAGNode* node = it->second;
//         // 在这里处理节点
//     }

//     return 0;
// }

int main()
{
    // 模拟命令行参数
    const char* mockArgv[] = {"programName", "arg1", "arg2", "arg3"};
    int mockArgc = sizeof(mockArgv) / sizeof(char*);

    auto moduleNameVec =
        OptionBase::parseOptions(mockArgc, const_cast<char**>(mockArgv), "Whole Program Points-to Analysis",
                                 "[options] <input-bitcode...>");


    // Refers to content of a singleton unique_ptr<SVFIR> in SVFIR.
    SVFIR* pag;

    if (Options::ReadJson())
    {
        pag = SVFIRReader::read(moduleNameVec.front());
    }
    else
    {
        if (Options::WriteAnder() == "ir_annotator")
        {
            LLVMModuleSet::preProcessBCs(moduleNameVec);
        }

        SVFModule* svfModule = LLVMModuleSet::buildSVFModule(moduleNameVec);

        /// Build SVFIR
        SVFIRBuilder builder(svfModule);
        pag = builder.build();

    }

    WPAPass wpa;
    wpa.runOnModule(pag);

    LLVMModuleSet::releaseLLVMModuleSet();
    return 0;
}