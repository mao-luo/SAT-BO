包含的部分

- sat文件夹包含walksat/dpll/check三个算法
  - walksat算法是sat启发式求解算法，main_example中有用法示例
  - dpll是完备求解算法
  - check是对解文件的判定，如果解文件中的若干解全部为sat，提示成功；如果解文件中一部分解为unsat，提示失败并返回unsat解的个数，main_example中有用法示例

- solve文件夹是用来储存输出的解文件

- var_weight文件夹用来储存变元权重，该文件夹是一个中间过程的文件夹
- specification_data：原始的数据文件

不包含的部分：

- 粒子群

- var_weight 的迭代更新

- 仿真环境（还需要进一步整理）