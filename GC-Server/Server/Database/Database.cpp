#include "Database.h"
#include <string.h>

Database::Database(const char* fileName)
{
	this->handle = nullptr;
	this->compiledStatement = nullptr;
	if (sqlite3_open_v2(fileName, &this->handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK)
		this->isOpen = true;
	else
		this->isOpen = false;
}

Database::~Database()
{
	sqlite3_close(this->handle);
}

int32_t Database::Execute(const char* statement)
{
	int32_t result;
	if ((result = sqlite3_prepare_v2(this->handle, statement, strlen(statement) + 1U, &this->compiledStatement, NULL)) != SQLITE_OK) // https://www.sqlite.org/c3ref/prepare.html
		return result;
	return sqlite3_step(this->compiledStatement);
}

bool Database::Next()
{
	if (sqlite3_step(this->compiledStatement) != SQLITE_ROW)
		return false;
	return true;
}

void Database::Finish()
{
	if (this->compiledStatement != nullptr)
		sqlite3_finalize(this->compiledStatement);
}

void Database::Test()
{
	sqlite3_close(this->handle);
}

int32_t Database::GetIntegerValue(int32_t index)
{
	if (this->compiledStatement == nullptr)
		return -1;
	return sqlite3_column_int(this->compiledStatement, index);
}

bool Database::IsOpen() const
{
	return this->isOpen;
}