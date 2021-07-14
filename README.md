# Simple_Qt_Parking_System
一个基于Qt5.9的简单停车场计费管理系统，用于结课作业。

## 说明

本项目基于Qt5.9与SQLite3，起源于一个可爱的小朋友结课作业DDL临近，于是在仅掌握C++基础的背景下自学Qt和SQLite3，硬肝了2天草草了事。程序中代码极其混乱，后续也不会进行重构与迭代，仅仅作为存档来记录这一段爆肝的时光。

如果你刚好有相同的课设作业，可以在留下一个Star后直接clone本项目进行魔改，无需保留作者信息！

以下内容均为设计程序时写下的部分信息，仅供本人参考。

## 参考

Qt学习之路2：https://www.devbean.net/2012/08/qt-study-road-2-catelog/

Qt快速入门：http://shouce.jb51.net/qt-beginning/

## 特别感谢

超级感谢 [@Kvar_ispw17](https://github.com/enkerewpo) 的指导与支持！！！

# 数据库设计与部分规范

## history

用于存储车辆入库出库行为。

| 字段名    | 类型 | 允许的值与说明             | 备注                       |
| --------- | ---- | -------------------------- | -------------------------- |
| id        | int  | 自增实数                   | 作为自增主键，用于标记事件 |
| action    | int  | 0 - 车辆入库，1 - 车辆出库 | 标记事件类型               |
| carid     | text | 按照车牌规范记录车牌号     | 作为车辆标识               |
| user_type | int  | 0 - 管理员，1 - 用户       | 记录操作执行者的权限       |
| time      | int  | 时间戳                     | 记录事件发生的时间         |
| stall_id  | int  | 对应stall表中的id          | 用于记录车辆所在的车位     |

## stall

用于存储车位信息，默认分为A、B区，各区共5个车位。

| 字段名 | 类型 | 允许的值与说明                                       | 备注             |
| ------ | ---- | ---------------------------------------------------- | ---------------- |
| id     | int  | 3位唯一实数，命名规则：区域ID（一位）+车位ID（两位） | 记录对应的车位   |
| area   | text | 1位字符，如A、B等，对应区域ID 01,02。                | 记录车位所在区域 |
| carid  | text | 按照车牌规范记录车牌号，无车辆时记录为NULL。         | 作为车辆标识     |

## finance

用于记录财务流水。

| 字段名 | 类型 | 允许的值与说明       | 备注                       |
| ------ | ---- | -------------------- | -------------------------- |
| id     | int  | 自增实数             | 作为自增主键，用于标记事件 |
| action | int  | 0 - 收入，1 - 支出   | 记录流水类型               |
| amount | int  | 全体整数             | 记录流水金额               |
| time   | int  | 时间戳               | 记录事件发生的时间         |
| carid  | int  | 按照车牌规范的车牌号 | 记录对应的出入库事件ID     |

## 车牌记录规范

为统一标准，在系统中采用以下车牌（carid）规范：

省简称及地区代号后的5位识别号（1-9+A-Z），例：湘A 123AB 则在录入系统时应输入：123AB

在实际操作中，我们需要对输入的车牌号进行格式化。

## 计费规范

收费标准：前1小时免费，超过每小时收费10元，48小时封顶。时间在运算过程中向上取整。

