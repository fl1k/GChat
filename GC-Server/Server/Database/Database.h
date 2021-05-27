#pragma once
#include <cstdint>
#include <sqlite3.h>

class Database
{
private:
	bool isOpen;
	sqlite3* handle;
	sqlite3_stmt* compiledStatement;

public:
	Database() = delete;
	Database(const char* fileName);
	~Database();
	int32_t Execute(const char* statement);
	bool Next();
	void Finish();
	void Test();
	int32_t GetIntegerValue(int32_t index);
	bool IsOpen() const;
};