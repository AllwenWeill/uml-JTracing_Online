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

/*todo list:1：增加switch解析d 增加while dowhile （foreach）等的支持；2：循环和判断数据加载；3：自调用数据加载和创建；4：if和else和loop的两种情况(花括号省略) 5:对elseif等语句的支持:5：隐藏的a.work
 * 需要判断当前类的成员函数，并解析（自调用情况或者头文件里的静态成员函数）实现对重名实例化的支持；不能实现直接的类解析
方案设想：1:自调用数据通过后继节点的层数判断 ，初始值设置为0，每次将自身直接后继节点的调用层数+1；一种减少循环的方法，classlist里面维护数组，一旦判断direct节点以后即将此节点加入禁用清单
 2：标记自身调用，invoke ==call 标记一次自调用并继续上找，否则不找了
 3 标记激活周期 根据子孙节点的序列号更新激活周期（最好在activate里激活）
active维护激活的序列号和此时的调用层数「
 方案一，面向箭头生成 维护单个call的自调用层数 方法如图方法2
 方案二、 面向类 维护一个表 key：激活时间 value：层数 实现方案：同样针对类更新激活时间，并imvoke ==call 寻找层数
 」
 4. 标记if/else 遇到if后判断是否需要括号匹配，若没有，解析语句，更新清单，若有执行括号匹配算法，分别统计执行前后的size，确定包含的
 5. 标记loop 同理
 Loop表维护
 ：size 从而维护了
 */