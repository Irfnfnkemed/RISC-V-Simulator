# RISC-V-Simulator
## 示意图
![](https://notes.sjtu.edu.cn/uploads/upload_b244585b61e11c267ffc514430fae619.jpg)

## 过程
1. 各个模块执行
2. 用修改缓存覆盖当前数据；

## 具体模块执行
### Reorder Buffer
若队头可提交，提交队头，将其从ROB中删去。若其为内存写指令，加入Load/Store Buffer缓冲区；其他情况，将其加入CDB缓冲区中。

特别的，若为强制跳转指令，将新的PC位置存入PC缓冲区中。若跳转预测错误，清空ROB,RS,Load/Store Buffer，Decoder以及RF中的依赖关系的缓冲区。若ROB处于不可添加状态，改为可添加状态。

若队尾可发射，结合RF，将其发射至RS中，其中LUI，AUIPC，JAL直接在ROB中进行运算。同时，根据RF情况，添加RF缓冲区中的依赖关系。

### Reservation Station
检查RS，将无依赖关系的指令加入ALU进行运算。

对于算完的指令，若为内存读指令，将其加入Load/Store Buffer缓冲区中；其他，将运算结果存入ROB的修改缓冲区，同时将ROB修改缓冲区的对应指令的状态改为ready，并利用计算出的值，更改RS缓冲区中的寄存器依赖关系。

### Load/Store Buffer
执行队列中的读/写操作。完成指令后，弹出队列。

若完成读指令，将其返回至ROB缓冲区中。

### Common Data Bus
删去RS缓冲区中的对应指令（依赖关系在ALU算完后已经更改）。

更改RF中的值，删去相应的依赖关系。

若为跳转指令，修改PC缓冲区的PC指向。

### Decoder
若ROB有空位且处于可添加状态，从内存中读取指令并进行解码，存入ROB中。

若为跳转指令且认为跳转（强制跳转、预测分支跳转），设置ROB缓冲区为不可添加状态。

