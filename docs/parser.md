待完善事件：
1. 解析位宽范围，例如：bit [3:0] a;(已完成)
2. 解析always_ff always_comb(已完成)
3. 解析if else（已完成）
4. VariableAST没有加入变量类型（已完成）
5. 解析（）圆括号内的表达式应不同于普通的表达式，需要重写（已完成）
6. 添加解析比较RHS函数及类(已完成)
7. 解析for循环
8. 解析initial(已完成)
9. 解析自增和自减，i++/++i/i--/--i(完成i++/i--)
10. 访问者模式？

问题：
1. 构建的变量类型表无法记录该变量的值?（或许在语法解析阶段不需要记录变量内容，仅需要记录变量是否出现过即可？）
	需要不断往后追踪,TypeFlag在第一次添加时不立刻还原，而接着判断后面Token是否为;分号还是=等号，如果分号则需要将TypeFlag还原NULL默认content为空，如果为等号，
	就在后面不断追踪ParseNumber等函数通过判断TypeFlag来为该标识符赋值。

2. 当前检测出语法的错误，可能会影响之后解析的判断，从而导致提供不正确的报错。
	由于解析过程较为复杂并且实际用户编写的代码过多不可确定性，该问题需要长时间补全corner cases。

3. getNextToken()中埋有潜在隐患，需要用++i效果更好，但修改需要大量调试，暂且搁置。(目前已影响解析begin)

SystemVerilog语法：
1. 变量：
reg	用户定义向量大小的通用4态变量；等价于var logic
logic	通常推断用户定义向量大小的通用var logic 4态变量，模块input/inout端口除外，在模块input/inout端口上推断wire logic
integer	32位4态状态变量；等价于var logic [ 31: 0 ]
bit	具有用户定义向量大小的通用2态var变量；如果未指定大小，则默认为1位大小
int	32位2态变量；相当于var bit[31 0]；综合编译器将int视为4态integer整数类型
byte	8位2态变量；等效于var bit [ 7 : 0 ]
shortint	16位2态变量；等效于var bit [ 15: 0 ]
longint	64位2态变量；等效于var bit [ 63: 0 ]

2. always_ff：用于描述触发器，进行非阻塞赋值。 always_comb：描述的是电路（组合逻辑电路）行为，用阻塞赋值。

已完成解析语法：
1.变量：reg、logic、integer、bit、int、byte、shortint、longint（含变量重定义检查）
2.module endmodule
3.二元表达式
4.数字
5.位宽，例如bit [3:0] a;
6.begin end
7.表达式
8.()
9.标识符
10.二元比较表达式
11.if-else
12.always_ff
13.always_comb
14.自增/自减
