# uml-JTracing

| **中文** | [English](readme_en.md) |

[![GitHubStars](https://img.shields.io/github/stars/AllwenWeill/uml-JTracing_Online?logo=GitHub)](https://github.com/AllwenWeill/uml-JTracing_Online/stargazers)
[![GitHub pull-requests](https://img.shields.io/github/issues-pr/AllwenWeill/uml-JTracing_Online.svg)](https://github.com/AllwenWeill/uml-JTracing_Online/pulls)
[![GitHub LICENSE](https://img.shields.io/github/license/AllwenWeill/uml-JTracing_Online.svg)](https://github.com/AllwenWeill/uml-JTracing_Online/blob/master/LICENSE)
[![GitHub contributors](https://img.shields.io/github/contributors/AllwenWeill/uml-JTracing_Online.svg)](https://github.com/AllwenWeill/uml-JTracing_Onlinee/graphs/contributors)

UML-JTracing是基于C++20实现，针对于芯片领域常用C++和SystemVerilog两种编程语言自动进行高鲁棒性的词法解析和常见语法分析和语义分析，生成可靠Abstract Syntax Tree，并提供Parser解析过程信息、报错信息和变量表等解析信息，通过自定义数据结构对解析过程进行格式化存储，最终将利用Python实现的UML智能生成器检测到结构化后的解析信息，通过加载解析信息进行自动绘制目标源代码的UML时序图

## 功能
* 利用有限自动机逐字符扫描C++17和SystemVerilog源代码进行高鲁棒性词法分析，稳定生成对应Token;
* 利用递归下降法手工构建语法分析器，实现C++17和SystemVerilog常见语法检查，生成分析结果并输出可靠AST抽象语法树；
* 提供解析过程信息、报错信息和变量表等解析信息，将源代码的执行流程进行特征提取，生成规范化的uml时序结构信息；
* 利用Turtle图形库动态绘制UML时序图，并支持对象和消息自定义；

## Demo演示
----------
> * __UML时序智能生成器整体演示__
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/github_func_demo.gif" height="429"/> </div>

> * __基于C++多文件真实项目案例测试__
> * *__<small>包含boss、workerManager等多个类，类与类之间存在互相调用和实例化，成员函数之间也存在多层嵌套关系，其中还包含各种for循环和while循环还有大量if else条件的使用</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/test_files.png" height="200"/> </div>

> * *__<small>提供解析过程信息</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/parsing_info.png" height="200"/> </div>

> * *__<small>提供UML时序结构信息：FuncTable</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/funcTable.png" height="200"/> </div>

> * *__<small>提供UML时序结构信息：FuncTable</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/funcTable.png" height="200"/> </div>

> * *__<small>提供UML时序结构信息：ActivationTable</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/ActivationTable.png" height="200"/> </div>

> * *__<small>提供UML时序结构信息：LoopTable</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/LoopTable.png" height="60"/> </div>

> * *__<small>提供UML时序结构信息：FuncTable</small>__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/AltTable.png" height="200"/> </div>

> * *__<small>UML时序图__*
<div align=center><img src="https://github.com/AllwenWeill/IMG/blob/main/UML.png" height="300"/> </div>

## 环境要求
* Windows/macOS环境
* C++17

## 项目启动
```bash
1. rebuild CMake->更新CMakeLists.txt
2. 在main.cpp中修改exePath为uml-JTracing_Online\\test\\output\\umlDrawing.exe的绝对路径
3. 在ClassList.cpp中修改m_ActivationTablePath、m_AltTablePath、m_LoopTablePath、m_FuncTablePath为uml-JTracing_Online\\test\\output\\对应.txt文件的绝对路径
4. 运行启动项UML-JTracing_Online.exe
(Input: 将需要分析的目标源文件放入Input文件夹中; Output: UML时序图及结构信息将生成在Output文件夹中.)
```


## 致谢
[@Ruslan Spivak](https://github.com/rspivak/): Let's Build A Simple Interpreter

[@LLVM](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html): My First Language Frontend with LLVM Tutorial
