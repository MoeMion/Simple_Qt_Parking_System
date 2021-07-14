#include "controller.h"
QString result_output ;
int stall_id = 0; //车位ID
int intime = 0; //入库时间
bool carid_check(char carid[])
{
	bool flag = 1;
	if ( strlen(carid) == 5 ) { 
		for (int i = 0; i < 5; i++) {
			if ( !((carid[i] >= 'A' && carid[i] <= 'Z') || (carid[i] >= '1' && carid[i] <= '9'))) {
				flag = 0;
			}
		}
	}
	else {
		flag = 0;
	}
	return flag;
}

bool database_check()
{
	sqlite3* db;
	int rc;
	rc = sqlite3_open(DBNAME, &db);
	if (rc) {
		return 0;
	}
	else {
		return 1;
	}
	sqlite3_close(db);
}


static int database_get_stall_callback(void* data, int argc, char** argv, char** azColName)
{
	if (atoi(argv[0])) {
		stall_id = atoi(argv[0]);
	}
	else {
		stall_id = 0;
	}
	return 1;
}

static int database_history_intime_callback(void* data, int argc, char** argv, char** azColName)
{
	if (atoi(argv[0])) {
		intime = atoi(argv[0]);
	}
	else {
		intime = 0;
	}
	return 1;
}

static int database_stall_listall_callback(void* data, int argc, char** argv, char** azColName)
{
    QString carstatus;
    if (argv[2] != NULL) { carstatus = QString(argv[2]); }
    else { carstatus = "暂无车辆"; } //argv[2]若为空指针会引起程序奔溃
    if (carstatus=="") { carstatus = "暂无车辆"; }
    result_output += "车库区域：" + QString(argv[1]) + " | 车位ID：" + QString(argv[0]) + " | 车牌号：" + carstatus + "\n";
	return 0;
}

static int database_finance_listall_callback(void* data, int argc, char** argv, char** azColName)
{
    QString finance_type;
    if (argv[1]) { finance_type = "支出"; }
    else { finance_type = "收入"; }
    result_output += "流水ID：" + QString(argv[0]) + " | 交易类型：" + finance_type +  " | 金额：" + QString(argv[2]) + " | 车牌号：" + QString(argv[4]) + " | 时间：" + QString(time_to_utc(atoi(argv[3])))  + "\n";
	return 0;
}

static int database_history_quire_callback(void* data, int argc, char** argv, char** azColName)
{
    QString action_type;
    if (atoi(argv[1])) { action_type = "取车"; }
    else { action_type = "存车"; }
    result_output += "记录ID：" + QString(argv[0]) + " | 操作类型：" + action_type + " | 车牌号：" + QString(argv[2]) + " | 车库ID：" + QString(argv[4]) + " | 时间：" + QString(time_to_utc(atoi(argv[3]))) + "\n";
	return 0;
}

int time_get_now()
{
	time_t t;
	return time(&t);
}

char* time_to_utc(int time)
{
	time_t t1 = time;
	return ctime(&t1);
}

int car_storage(char carid[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
	stall_id = 0;
	stall_get_id(carid);
    if (stall_id != 0) { return 1; }
	stall_id = 0;
	stall_get_free();
	int freestall = stall_id;
    if (!freestall) { return 0; }
	rc = sqlite3_open(DBNAME, &db);
    QString sql_str = "INSERT INTO history (action,carid,time,stall_id) VALUES (0, '"+QString(carid)+"', "+QString::number(time_get_now())+", "+QString::number(freestall)+"); ";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	sqlite3_close(db);
	stall_store(freestall,carid);
	return freestall;
}

int car_fetch(char carid[])
{
	stall_id = 0;
	stall_get_id(carid);
	if (stall_id == 0) { return 0; }
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
	rc = sqlite3_open(DBNAME, &db);
    QString sql_str = "INSERT INTO history (action,carid,time,stall_id) VALUES (1, '"+QString(carid)+"', "+QString::number(time_get_now())+", "+QString::number(stall_id)+"); ";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	sqlite3_close(db);
	stall_clear(stall_id);
	intime = 0;
	history_intime(carid,stall_id);
    return intime ? time_get_now()-intime:intime ;
}

int stall_get_free()
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	const char* sql= "SELECT * from stall where carid is '' or carid is null;";
	rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_get_stall_callback,NULL, &zErrMsg);
	sqlite3_close(db);
	return 0;
}

int stall_get_id(char carid[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
    QString sql_str = "SELECT * from stall where carid is '"+QString(carid)+"';";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_get_stall_callback, NULL, &zErrMsg);
	sqlite3_close(db);
	return 0;
}

void stall_store(int stallid,char carid[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
	rc = sqlite3_open(DBNAME, &db);
    QString sql_str = "update stall set carid = '"+QString(carid)+"' where id = "+QString::number(stallid)+" ;";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	sqlite3_close(db);
}

void stall_clear(int stallid)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
	rc = sqlite3_open(DBNAME, &db);
    QString sql_str = "update stall set carid = '' where id = "+QString::number(stallid)+" ;";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	sqlite3_close(db);
}

void stall_listall()
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	const char* sql = "SELECT * from stall;";
	rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_stall_listall_callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void history_intime(char carid[], int stallid)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
    QString sql_str = "SELECT time from history where carid = '"+QString(carid)+"' and stall_id = "+QString::number(stallid)+" and action = 0 order by time desc;";
    qstr2cstr(sql_str,sql);
    rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_history_intime_callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void history_quire(char carid[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[100];
    QString sql_str = "SELECT * from history where carid = '"+QString(carid)+"';";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_history_quire_callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

int finance_compute(int parktime)
{
	int amount = 0;
	int t = parktime;
	parktime /= 3600;
	if (t % 3600 != 0) { parktime++; }
	if (parktime <= 1) { amount = 0; }
	else if (parktime <= 48) { amount = 10 * (parktime-1); }
	else { amount = 480; }
	return amount;
}

void finance_check(int amount, char carid[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char sql[500];
	rc = sqlite3_open(DBNAME, &db);
    QString sql_str = "INSERT INTO finance (action,amount,time,carid) VALUES (0, "+QString::number(amount)+", "+QString::number(time_get_now())+", '"+QString(carid)+"'); ";
    qstr2cstr(sql_str,sql);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	sqlite3_close(db);
}

void finance_listall()
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	const char* sql = "SELECT * from finance;";
	rc = sqlite3_open(DBNAME, &db);
	rc = sqlite3_exec(db, sql, database_finance_listall_callback, NULL, &zErrMsg);
	sqlite3_close(db);
}

void qstr2cstr(QString qstr,char cstr[]){
    std::string sstr;
    sstr = qstr.toStdString();
    strcpy(cstr,sstr.c_str());
}
