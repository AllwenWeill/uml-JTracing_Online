![image](https://github.com/AllwenWeill/uml-JTracing_Online/assets/53572103/cb3e913e-993f-4dfb-8a0c-940d3a740661)# uml-JTracing

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
