#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <algorithm>
#include <map>

enum class DataType {
    INT,
    STRING,
    FLOAT,
    DOUBLE,
    DATE
};

auto Menu()->void;
auto toLowercase2(std::string str) -> std::string;
auto toUppercase(std::string str)->std::string;
auto stringToDataType(const std::string &type)->DataType;

struct Column {
    std::string name;
    DataType type;
    Column(const std::string& name,const DataType& type) {
        this->name = toUppercase(name);
        this->type = type;
    }
    std::string getName() const {return name;}
    DataType getType() const {return type;}
    void setType(std::string type) {Column::type = stringToDataType(type);}
    void setName(std::string name) {Column::name = name;}
};

struct Row{
    std::map<std::string,std::string> values;
};

struct Table {
    std::string name;
    std::vector<Row> rows;
    std::vector<Column> columns;
    std::map<std::string, Column> primaryKey;

    Table() {this->name = "";} // to be able to create a table without a name and without initializing values which will be done later
    Table(const std::string &name){this->name = name;}
    std::string getName() const {return name;}
    void setName(std::string name) {Table::name = name;}
    std::vector<Row>& getRows() {return rows;} // to be able to modify rows add or modify a row
    const std::vector<Row>& getRows() const {return rows;} //to be able to access without modifying
    void setRows(std::vector<Row> rows) {Table::rows = rows;}
    const std::vector<Column>& getColumns() const {return columns;}
    void setColumns(std::vector<Column> columns) {Table::columns = columns;}
    const std::map<std::string, Column>& getPrimaryKey() const {return this->primaryKey;}

    auto getColumn(const std::string& columnName) const->Column;
    auto addColumn(const Column &c)->void;
    auto removeColumn(const std::string& name)->void;
    auto addPrimaryKey(const std::string& keyName, const Column& primaryKey)->void;
    auto printtable()->void;
    auto printtable(const std::vector<std::string>& columnNames)->void;
    auto printRow(const Row& row, const std::vector<std::string>& columnNames)const->void;
    auto setColumnValueForAllRows(const std::string& columnName, const std::string& value)->void;
};

struct Database{
    std::map<std::string,Table> tables;

    std::map<std::string, Table> &getTables()  {return tables;}
    void setTables(const std::map<std::string, Table> tables) {Database::tables = tables;}
    auto dropTable(const std::string& name)->void;
    auto addTable(const Table& table)->void;
    auto insertColumns(const std::string& name, const Column& column)->void;
    auto saveTableToFile(const std::string& tableName, const std::string& folderPath)->void;
    auto loadTableFromFile(const std::string& tableName, const std::string& folderPath)->void;
    auto selectFromTable(const std::vector<std::string>& columnNames, const std::string& tableName, const std::string& condition)->void;
    auto evaluateCondition(const std::string& tableName, const Row& row, const std::vector<std::string>& conditionParts)->bool;
    auto executeCommand(const std::string& command)->void;
    auto addValuesToRow(const std::string& tableName, const std::vector<std::string>& columnValues)->bool;
    auto updateTable(const std::string& tableName, const std::string& columnName, const std::string& value, const std::string& condition)->void;
    auto deleteFromTable(const std::string& tableName, const std::string& condition)->void;
    auto deleteFromTable(const std::string& tableName)->void;


};















