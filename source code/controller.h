#pragma once
#ifndef __CONTROLLER_HEADER__
#define __CONTROLLER_HEADER__  
#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <QString>
#include "sqlite3.h"
#define DBNAME "db.db"

extern QString result_output;
extern int stall_id; //车位ID
extern int intime; //入库时间
bool carid_check(char carid[]); //检查车牌是否合法
bool database_check(); //检查数据库连接是否正常
static int database_get_stall_callback(void* data, int argc, char** argv, char** azColName); //获取空闲车位回调函数
static int database_history_intime_callback(void* data, int argc, char** argv, char** azColName); //获取入库时间回调函数
static int database_stall_listall_callback(void* data, int argc, char** argv, char** azColName); //获取所有车位状态回调函数
static int database_finance_listall_callback(void* data, int argc, char** argv, char** azColName); //获取所有财务记录回调函数
static int database_history_quire_callback(void* data, int argc, char** argv, char** azColName); //查询某辆车所有出入记录回调函数
int time_get_now(); //获取时间戳
char* time_to_utc(int time); //时间戳转UTC格式时间
int car_storage(char carid[]); //车辆入库，返回所在的车库ID，若无空闲车位则返回0，车辆已存在则返回1
int car_fetch(char carid[]); //车辆出库，成功返回总停车秒数，车辆不存在返回0
int stall_get_free(); //获取空闲的车位ID，若无空闲则返回0
int stall_get_id(char carid[]); //获取目标汽车的车位ID
void stall_store(int stallid, char carid[]); //标记入库的车位信息
void stall_clear(int stallid); //标记出库的车位信息
void stall_listall(); //列出当前所有车位状态
void history_intime(char carid[], int stallid); //获取车辆入库时间
void history_quire(char carid[]); //查询某辆车所有出入记录
int finance_compute(int parktime); //根据停车秒数计算停车费用
void finance_check(int amount, char carid[]); //记录财务结算信息
void finance_listall(); //列出所有财务记录
void qstr2cstr(QString qstr,char cstr[]); //将Qstring类型转为char型字符串数组

#endif // !__CONTROLLER_HEADER__
