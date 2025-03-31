#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <regex>
#include "Classes_And_Functions.h"

auto validateDataType(const std::string& value, DataType type)->bool;
auto dataTypeToString(DataType type)-> std::string;

//Helper Functions
auto toLowercase2(std::string str) -> std::string { //to use the output again somewhere else
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
auto toLowercase(std::string& str) -> void {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}
auto toUppercase(std::string str)->std::string{ //to use the output again somewhere else
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
auto trimSpaces(std::string str)->std::string {
    str.erase(str.begin(),
              std::find_if(str.begin(), str.end(), [](unsigned char ch) {
                  return !std::isspace(ch);
              }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

auto dataTypeToString(const DataType type)-> std::string {
    switch (type) {
        case DataType::INT: return "INT";
        case DataType::STRING: return "STRING";
        case DataType::FLOAT: return "FLOAT";
        case DataType::DOUBLE: return "DOUBLE";
        case DataType::DATE: return "DATE";
        default:throw std::invalid_argument("Unsupported data type");
    }
}

auto stringToDataType(const std::string &type)->DataType {
    std::string typeUpper = toUppercase(type);
    if (typeUpper == "INT") {return DataType::INT;}
    else if (typeUpper == "STRING") {return DataType::STRING;}
    else if (typeUpper == "FLOAT") {return DataType::FLOAT;}
    else if (typeUpper == "DOUBLE") {return DataType::DOUBLE;}
    else if (typeUpper == "DATE") {return DataType::DATE;}
    else {throw std::invalid_argument("Invalid data type: " + type);}
}

auto validateDataType(const std::string& value, DataType type)->bool {
    try {
        switch (type) {
            case DataType::INT:
                std::stoi(value);
                break;
            case DataType::FLOAT:
                std::stof(value);
                break;
            case DataType::DOUBLE:
                std::stod(value);
                break;
            case DataType::DATE: {
                // Detailed date validation (YYYY-MM-DD)
                if (value.size() != 10 || value[4] != '-' || value[7] != '-') {
                    throw std::invalid_argument("Invalid date format");
                }
                int year = std::stoi(value.substr(0, 4));
                int month = std::stoi(value.substr(5, 2));
                int day = std::stoi(value.substr(8, 2));
                if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
                    throw std::invalid_argument("Invalid date value");
                }
                break;
            }
            case DataType::STRING:
                break;
            default:
                throw std::invalid_argument("Unsupported data type");
        }
    } catch (const std::invalid_argument&) {
        return false;
    }
    return true;
}

//TABLE CLASS FUNCTIONS
auto Table::addColumn(const Column &c)->void{Table::columns.push_back(c);}
auto Table::addPrimaryKey(const std::string& keyName, const Column& primaryKey)->void {
    this->primaryKey.insert({keyName, primaryKey});
}
auto Table::setColumnValueForAllRows(const std::string& columnName, const std::string& value)->void {
    for (auto& row : rows) {
        row.values[columnName] = value;
    }
}
auto Table::removeColumn(const std::string& name)->void  {
    auto it = std::find_if(columns.begin(),columns.end(),[&name](const Column& c){return c.getName() == name;});
    if(it != columns.end()){
        columns.erase(it);
    }
}
auto Table::getColumn(const std::string& columnName) const->Column {
    auto it = std::find_if(columns.begin(), columns.end(), [&columnName](const Column& col) { return col.getName() == columnName; });
    if (it != columns.end()) {
        return *it;
    } else {
        throw std::runtime_error("Column not found");
    }
}

auto Table::printtable()->void { //function to print table
    std::string separator = "+------------+";
    for (auto i = 1; i < getColumns().size(); i++) {
        separator += "-+-----------------+";
    }
    fmt::println("{}", separator);

    for (auto i = 0; i < getColumns().size(); i++) {
        if (i == 0) {
            fmt::print("|{:<12}| ", getColumns()[i].getName());
        } else {
            fmt::print("|{:<17}| ", getColumns()[i].getName());
        }
    }
    fmt::println("");
    fmt::println("{}", separator);

    for (const auto &row: getRows()) {
        for (auto i = 0; i < getColumns().size(); ++i) {
            if (i == 0) {
                fmt::print("|{:>12}| ", row.values.at(getColumns()[i].getName()));
            } else {
                const std::string &value = row.values.at(getColumns()[i].getName());
                if (value.empty()) {
                    fmt::print("|{:<17}| ", "");
                } else if (std::all_of(value.begin(), value.end(), ::isdigit)) {
                    fmt::print("|{:>17}| ", value);
                } else {
                    fmt::print("|{:<17}| ", value);
                }
            }
        }
        fmt::println("");
        fmt::println("{}",separator);
    }
}

auto Table::printtable(const std::vector<std::string>& columnNames)->void { //function to print table with specific columns
    std::string separator = "+------------+";
    for (auto i = 1; i < columnNames.size(); i++) {
        separator += "-+-----------------+";
    }
    fmt::println("{}", separator);

    for (auto i = 0; i < columnNames.size(); i++) {
        if (i == 0) {
            fmt::print("|{:<12}| ", columnNames[i]);
        } else {
            fmt::print("|{:<17}| ", columnNames[i]);
        }
    }
    fmt::println("");
    fmt::println("{}", separator);

    for (const auto &row: getRows()) {
        for (auto i = 0; i < columnNames.size(); ++i) {
            if (i == 0) {
                fmt::print("|{:>12}| ", row.values.at(getColumns()[i].getName()));
            } else {
                const std::string &value = row.values.at(columnNames[i]);
                if (value.empty()) {
                    fmt::print("|{:<17}| ", "");
                } else if (std::all_of(value.begin(), value.end(), ::isdigit)) {
                    fmt::print("|{:>17}| ", value);
                } else {
                    fmt::print("|{:<17}| ", value);
                }
            }
        }
        fmt::println("");
        fmt::println("{}", separator);
    }
}

auto Table::printRow(const Row& row, const std::vector<std::string>& columnNames)const->void{ //function to print a single row
    std::string separator = "+------------+";
    for(auto i = 1; i < columnNames.size(); i++){
        separator += "-+-----------------+";
    }

    fmt::println("{}",separator);
    for(auto i = 0; i < columnNames.size();i++){
        if(i==0){
            fmt::print("|{:<12}| ", columnNames[i]);
        }else{
            fmt::print("|{:<17}| ", columnNames[i]);
        }
    }
    fmt::println("");
    fmt::println("{}",separator);

    for (auto i = 0; i < columnNames.size(); ++i) {
        if (i == 0) {
            fmt::print("|{:>12}| ", row.values.at(columnNames[i]));
        } else {
            const std::string& value = row.values.at(columnNames[i]);
            if (value.empty()) {
                fmt::print("|{:<17}| ", "");
            } else if (std::all_of(value.begin(), value.end(), ::isdigit)) {
                fmt::print("|{:>17}| ", value);
            } else {
                fmt::print("|{:<17}| ", value);
            }
        }
    }
    fmt::println("");
    fmt::println("{}",separator);
}

//DATABASE CLASS FUNCTIONS
auto Database::insertColumns(const std::string& name, const Column& column)->void{
    tables[name].addColumn(column);
}
auto Database::addTable(const Table& table)->void{
    tables[table.getName()] = table;
}
auto Database::dropTable(const std::string& name)->void{
    tables.erase(name);
}
auto Database::selectFromTable(const std::vector<std::string>& columnNames, const std::string& tableName, const std::string& condition)->void{
    if (tables.find(tableName) == tables.end()) {
        fmt::println("Table '{}' does not exist.", tableName);
        return;
    }
    const Table& table = tables[tableName];

    if (condition.empty()) {
        if (columnNames[0] == "*") {
            getTables()[tableName].printtable();
        } else {
            getTables()[tableName].printtable(columnNames);
        }
    } else {
        std::vector<std::string> conditionParts;
        std::istringstream iss(condition);
        for(std::string s; iss >> s; ) conditionParts.push_back(s);

        std::vector<Row> selectedRows;
        for (const auto& row : table.getRows()) {
            if (evaluateCondition(tableName, row, conditionParts)) {
                selectedRows.push_back(row);
            }
        }

        Table tempTable = table;
        tempTable.setRows(selectedRows);

        if (columnNames[0] == "*") {
            tempTable.printtable();
        } else {
            tempTable.printtable(columnNames);
        }
    }
}

auto Database::addValuesToRow(const std::string& tableName, const std::vector<std::string>& columnValues)->bool {
    if (tables.find(tableName) == tables.end()) {
        fmt::println("Table {} does not exist.", tableName);
        return false;
    }

    Table& table = tables.at(tableName);
    if (table.getColumns().size() != columnValues.size()) {
        fmt::println("Number of columns and values must match!");
        return false;
    }

    Row newRow;
    for (auto i = 0; i < table.getColumns().size(); i++) {
        const auto &column = table.getColumns().at(i);
        const auto &value = columnValues.at(i);
        const auto &columnName = column.getName(); // Assuming column names are stored in uppercase

        if (!validateDataType(value, column.getType())) {
            fmt::println("Invalid value for column {}: expected {} type, got {}", columnName, dataTypeToString(column.getType()), value);
            return false;
        }

        bool isPrimaryKey = false;
        for (const auto& pair : table.getPrimaryKey()) {
            if (pair.second.getName() == columnName) {
                isPrimaryKey = true;
                break;
            }
        }
        if (isPrimaryKey) {
            for (const auto& row : table.getRows()) {
                if (row.values.at(columnName) == value) {
                    fmt::println("Cant insert row. duplicate primary key value '{}'.", value);
                    return false;
                }
            }
        }
        newRow.values[column.getName()] = value;
    }
    table.getRows().push_back(newRow);
    return true;
}

auto Database::updateTable(const std::string& tableName, const std::string& columnName, const std::string& value, const std::string& condition)->void {
    if (tables.find(tableName) == tables.end()) {
        fmt::println("Table '{}' does not exist.", tableName);
        return;
    }

    Table& table = tables[tableName];
    try {
        table.getColumn(columnName);
    } catch (const std::runtime_error&) {
        fmt::println("Column '{}' not found '{}'.", columnName, tableName);
        return;
    }

    std::vector<std::string> conditionParts;
    std::istringstream iss(condition);
    for(std::string s; iss >> s; )
        conditionParts.push_back(s);

    for (auto& row : table.getRows()) {
        if (condition.empty() || evaluateCondition(tableName, row, conditionParts)) {
            row.values[columnName] = value;
        }
    }
}

auto Database::deleteFromTable(const std::string& tableName, const std::string& condition)->void {
    if (tables.find(tableName) == tables.end()) {
        fmt::println("Table '{}' does not exist.", tableName);
        return;
    }

    Table& table = tables[tableName];
    if (condition.empty()) {
        table.getRows().clear();
        return;
    }

    std::vector<std::string> conditionParts;
    std::istringstream iss(condition);
    for(std::string s; iss >> s; )
        conditionParts.push_back(s);

    for (auto it = table.getRows().begin(); it != table.getRows().end(); ) {
        if (evaluateCondition(tableName, *it, conditionParts)) {
            it = table.getRows().erase(it);
        } else {
            ++it;
        }
    }
}

auto Database::deleteFromTable(const std::string& tableName)->void {
    if (tables.find(tableName) == tables.end()) {
        fmt::println("Table '{}' does not exist.", tableName);
        return;
    }
    Table& table = tables[tableName];
    table.getRows().clear();
}

auto Database::saveTableToFile(const std::string& tableName, const std::string& folderPath)->void {
    std::string filePath = folderPath + "\\" + tableName + ".txt";

    auto outFile = std::fstream(filePath, std::ios::out| std::ios::trunc);
    if (!outFile) {
        fmt::println("Unable to open file for saving.");
        return;
    }
    const Table& table = tables[tableName];

    // write create table command to file
    outFile << "create table " << toLowercase2(table.getName()) << " (";
    for (size_t i = 0; i < table.getColumns().size(); ++i) {
        outFile << toLowercase2(table.getColumns()[i].getName()) << " " << toLowercase2(dataTypeToString(table.getColumns()[i].getType()));
        if (i < table.getColumns().size() - 1) {
            outFile << ",";
        }
    }

    // Write the primary key if it exists
    if (!table.getPrimaryKey().empty()) {
        outFile << ") constraint ";
        for (const auto& pair : table.getPrimaryKey()) {
            outFile << pair.first << " primary key (" << toLowercase2(pair.second.getName()) << ")";
        }
    }
    outFile << "\n";

    // Write the insert into command to file
    for (const auto& row : table.getRows()) {
        outFile << "insert into " << toLowercase2(table.getName()) << " (";
        for (auto i = 0; i < table.getColumns().size(); ++i) {
            outFile << toLowercase2(table.getColumns()[i].getName());
            if (i < table.getColumns().size() - 1) {
                outFile << ", ";
            }
        }
        outFile << ") values (";
        for (auto i = 0; i < table.getColumns().size(); ++i) {
            outFile << row.values.at(table.getColumns()[i].getName()) ;
            if (i < table.getColumns().size() - 1) {
                outFile << ", ";
            }
        }
        outFile << ")\n";
    }
    outFile.close();
}

auto Database::loadTableFromFile(const std::string& tableName, const std::string& folderPath)->void {
    std::string fileName = tableName + ".txt";
    std::string filePath;

    // Get the current directory
    std::filesystem::path workspaceRoot = std::filesystem::current_path();

    // Combine the current directory with the folder path provided by the user
    std::filesystem::path path = workspaceRoot / std::filesystem::path(folderPath);

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        if (entry.is_regular_file() && entry.path().filename() == fileName) {
            filePath = entry.path().string();
            break;
        }
    }

    if (!std::filesystem::exists(filePath)) {
        fmt::println("File '{}' does not exist.", filePath);
        return;
    }

    std::ifstream inFile(filePath);
    if (!inFile) {
        fmt::println("Unable to open file for loading.");
        return;
    }

    // Read each line from the file and execute it as a command
    std::string command;
    while (std::getline(inFile, command)) {
        try {
            executeCommand(command);
        }catch (const std::exception& e) {
            fmt::println("Error executing command: {}", e.what());
        }
    }
    inFile.close();
    fmt::println("Commands executed successfully from file '{}'", fileName);
}

auto Database::evaluateCondition(const std::string& tableName, const Row& row, const std::vector<std::string>& conditionParts)->bool{
    std::string column = conditionParts[0];
    std::string op = conditionParts[1];
    std::string value = conditionParts[2];
    value.erase(std::remove(value.begin(), value.end(), '\''), value.end()); // Remove single quotes from value

    std::transform(column.begin(), column.end(), column.begin(), ::toupper);

    if (row.values.find(column) == row.values.end()) {
        fmt::print("Column '{}' does not exist in row.", column);
        return false;
    }

    const std::string& rowValue = row.values.at(column); //value
    const Table& table = this->getTables().at(tableName); //table
    const DataType type = table.getColumn(column).getType(); //datatype

    if (type == DataType::STRING) {
        if (op == "=") {return rowValue == value;}
        else if (op == "!=") {return rowValue != value;}
    } else if (type == DataType::INT || type == DataType::FLOAT || type == DataType::DOUBLE) {
        double rowValueNum = std::stod(rowValue); //value from row
        double valueNum = std::stod(value); //value from condition
        if (op == "=") {return rowValueNum == valueNum;}
        else if (op == "!=") {return rowValueNum != valueNum;}
        else if (op == "<") {return rowValueNum < valueNum;}
        else if (op == "<=") {return rowValueNum <= valueNum;}
        else if (op == ">") {return rowValueNum > valueNum;}
        else if (op == ">=") {return rowValueNum >= valueNum;}
    } else if (type == DataType::DATE) {
        if (op == "=") {return rowValue == value;}
        else if (op == "!=") {return rowValue != value;}
        else if (op == "<") {return rowValue < value;}
        else if (op == "<=") {return rowValue <= value;}
        else if (op == ">") {return rowValue > value;}
        else if (op == ">=") {return rowValue >= value;}
    }
    return false;
}

auto Database::executeCommand(const std::string& command)->void {
    std::istringstream csstream(command);
    std::vector<std::string> words((std::istream_iterator<std::string>(csstream)),
                                   std::istream_iterator<std::string>()); //vector to take input stream from csstream and store it in words

    // Convert the command words to lowercase except for the values part in insert into because we want to keep the values as they are
    for (auto &word: words) {
        if (word != "values") {
            toLowercase(word);
        } else {
            break;
        }
    }
    //exit
    if (words[0] == "exit") {
        exit(0);
    }
        //help
    else if (words[0] == "help") {
        fmt::println("{}", " List Of Commands: ");
        fmt::println("{}", "List Of DataTypes");
        fmt::println("{}", "Create Table table_name (column1 datatype,column2 datatype,column3 datatype, ...)");
        fmt::println("{}", "Create Table table_name (column1 datatype,column2 datatype,column3 datatype) constraint constraint_name primary key (column1)");
        fmt::println("{}", "Drop Table table_name");
        fmt::println("{}", "Insert Into table_name (column1,column2,column3, ...) values (value1,value2,value3, ...)");
        fmt::println("{}", "Alter table table_name add column_name datatype");
        fmt::println("{}", "Alter table table_name drop column_name");
        fmt::println("{}", "Update table_name set column_name = value where condition");
        fmt::println("{0}{1}", "Delete from table_name where condition", " or Delete from table_name");
        fmt::println("{0}{1}", "Select column1,column2,column3,.... from table_name", " or Select * from table_name");
        fmt::println("{}", "Select column1,column2,column3,..../* from table_name where condition1 ");
        fmt::println("{}", "All tables");
        fmt::println("{}", "Save table_name");
        fmt::println("{}", "Load table_name");
    }
        //list of data types
    else if (words[0] == "list" && words[1] == "of" && words[2] == "datatypes") {
        fmt::println("{}", "INT (MAX 10 DIGITS)");
        fmt::println("{}", "STRING");
        fmt::println("{}", "FLOAT");
        fmt::println("{}", "DOUBLE");
        fmt::println("{}", "DATE (YYYY-MM-DD)");
    }
        //create table
    else if (words[0] == "create" && words[1] == "table" && words.size() > 2) {
        std::regex createTableRegex(
                R"(create\s+table\s+(\w+)\s*\((.*?)\)\s*(constraint\s+(\w+)\s+primary\s+key\s*\((\w+)\))?)",
                std::regex::icase);
        std::smatch createTableMatch;

        if (std::regex_match(command, createTableMatch, createTableRegex)) {
            auto tableName = toUppercase(createTableMatch[1].str());
            auto table = Table(tableName);
            auto columnsStr = createTableMatch[2].str();

            // Split the column definitions by comma
            std::istringstream colsStream(columnsStr);
            std::string columnPart;
            while (std::getline(colsStream, columnPart, ',')) {
                // Remove leading and trailing spaces from columnDefinition
                columnPart = trimSpaces(columnPart);

                // Split the column definition by space to separate the column name and data type
                std::istringstream ColumnsStream(columnPart);
                std::vector<std::string> columnParts;
                std::string part;
                while (ColumnsStream >> part) {
                    columnParts.push_back(part);
                }
                if (columnParts.size() != 2) {
                    fmt::println("Invalid column definition: {}", columnPart);
                    return;
                }
                auto columnName = toUppercase(columnParts[0]);
                auto columnTypeStr = columnParts[1];
                DataType columnType;
                try {
                    columnType = stringToDataType(columnTypeStr);
                } catch (const std::invalid_argument &) {
                    fmt::println("Invalid data type: {}", columnTypeStr);
                    return;
                }

                auto column = Column(columnName, columnType);
                table.addColumn(column);
            }

            // Check if the command contains the 'primary key' keyword
            if (createTableMatch[3].matched) {
                auto pkName = createTableMatch[4].str();
                auto pkColumn = toUppercase(createTableMatch[5].str());
                auto it = std::find_if(table.getColumns().begin(), table.getColumns().end(),
                                       [&pkColumn](const Column &column) {
                                           return column.getName() == pkColumn;
                                       });
                if (it != table.getColumns().end()) {
                    table.addPrimaryKey(pkName, *it);
                } else {
                    fmt::println("Primary key {} does not match any column", pkColumn);
                    return;
                }
            }
            addTable(table);
            fmt::println("Table {} created successfully", toUppercase(tableName));
        }
    }
        //all tables
    else if (words[0] == "all" && words[1] == "tables") {
        for (auto &table: tables) {
            fmt::print("{}\n", table.first);
            table.second.printtable();

            // primary key
            const auto &primaryKey = table.second.getPrimaryKey();
            if (!primaryKey.empty()) {
                fmt::print("Primary keys: ");
                fmt::println("{}", primaryKey.begin()->first);
                for (const auto &pair: primaryKey) {
                    fmt::print("{} ({}) ", pair.second.getName(),
                               dataTypeToString(pair.second.getType())); // Print the column name and its DataType
                }
                fmt::println("");
            }

        }
    }
        //drop table
    else if (words[0] == "drop" && words[1] == "table" && words.size() > 2) {
        auto tableName = toUppercase(words[2]);
        dropTable(tableName);
        fmt::println("Table {} dropped successfully", toUppercase(tableName));
    }
        //insert into
    else if (words[0] == "insert" && words[1] == "into" && words.size() > 3) {
        std::regex insertPattern(R"(insert\s+into\s+(\w+)\s*\((.+)\)\s*values\s*\((.+)\))",
                                 std::regex_constants::icase);
        std::smatch matches;

        if (std::regex_search(command, matches, insertPattern) && matches.size() == 4) {
            auto tableName = toUppercase(matches[1].str());
            auto columnNamesStr = matches[2].str();
            auto valuesStr = matches[3].str();

            std::istringstream Columns(columnNamesStr);
            std::vector<std::string> columnNames;
            std::string columnName;
            while (std::getline(Columns, columnName, ',')) {
                columnNames.push_back(toUppercase(columnName));
            }

            std::istringstream ValuesStream(valuesStr);
            std::vector<std::string> values;
            std::string value;
            while (std::getline(ValuesStream, value, ',')) {
                // Remove leading and trailing spaces from value
                value = trimSpaces(value);
                values.push_back(value);
            }
            if (columnNames.size() != values.size()) {
                fmt::println("Number of columns and values must match!");
                return;
            }
            bool rowInserted = addValuesToRow(tableName, values);
            if (rowInserted) {
                fmt::println("Row inserted successfully into table {}", tableName);
            }
        }
    }
        //alter table add column
    else if (words[0] == "alter" && words[1] == "table" && words[3] == "add" && words.size() > 5) {
        auto tableName = toUppercase(words[2]);
        auto columnName = toUppercase(words[4]);
        auto dataTypeStr = words[5];

        DataType dataType;
        try {
            dataType = stringToDataType(dataTypeStr);
        } catch (const std::invalid_argument &) {
            fmt::println("Invalid data type: {}", dataTypeStr);
            return;
        }
        auto column = Column(columnName, dataType);
        insertColumns(tableName, column);
        getTables()[tableName].setColumnValueForAllRows(columnName, "");

        fmt::println("Column {} added successfully to table {}", columnName, tableName);
    }
        //alter table drop column
    else if (words[0] == "alter" && words[1] == "table" && words[3] == "drop" && words.size() > 4) {
        auto tableName = toUppercase(words[2]);
        auto columnName = toUppercase(words[4]);
        getTables()[tableName].removeColumn(columnName);

        fmt::println("Column {} dropped successfully from table {}", columnName, tableName);
    }
        //update operation with  condition
    else if (words[0] == "update" && words[2] == "set" && words.size() > 4) {
        auto tableName = toUppercase(words[1]);
        auto columnName = toUppercase(words[3]);
        auto value = words[5];

        const Table &table = this->getTables().at(tableName);
        const DataType type = table.getColumn(columnName).getType();
        if (!validateDataType(value, type)) {
            fmt::print("Invalid value for column '{}': expected {} type, got '{}'", columnName, dataTypeToString(type),
                       value);
            return;
        }

        // Construct the condition
        std::string condition;
        auto wherePos = std::find(words.begin(), words.end(), "where");
        if (wherePos != words.end()) {
            for (auto it = wherePos + 1; it != words.end(); ++it) {
                condition += *it + " ";
            }
            condition.erase(condition.find_last_not_of(" \n\r\t") + 1);
            updateTable(tableName, columnName, value, condition);
        }
    }
        //delete operation with or without condition
    else if (words[0] == "delete" && words[1] == "from" && words.size() > 2) {
        auto tableName = toUppercase(words[2]);
        if (getTables().find(tableName) == getTables().end()) {
            fmt::print("Table '{}' does not exist.", tableName);
            return;
        }
        std::string condition;
        auto wherePos = std::find(words.begin(), words.end(), "where");
        if (wherePos != words.end()) {
            for (auto it = wherePos + 1; it != words.end(); ++it) {
                condition += *it + " ";
            }
            condition.erase(condition.find_last_not_of(" \n\r\t") + 1);
            deleteFromTable(tableName, condition);
        } else {
            deleteFromTable(tableName);
        }
    }
        //select operation with or without condition
    else if (words[0] == "select" && words[2] == "from" && words.size() > 3) {
        auto tableName = toUppercase(words[3]);
        auto columnNamesStr = words[1];

        // Split the column names by comma
        std::istringstream ColumnsStream(columnNamesStr);
        std::vector<std::string> columnNames;
        std::string columnName;
        while (std::getline(ColumnsStream, columnName, ',')) {
            // Trim spaces from the column name
            columnName = trimSpaces(columnName);
            columnNames.push_back(toUppercase(columnName));
        }

        std::string condition;
        auto wherePos = std::find(words.begin(), words.end(), "where");
        if (wherePos != words.end()) {
            for (auto it = wherePos + 1; it != words.end(); ++it) {
                condition += *it + " ";
            }
            condition.erase(condition.find_last_not_of(" \n\r\t") + 1);
        }

        selectFromTable(columnNames, tableName, condition);
    }


        //save table
    else if (words[0] == "save" && words.size() > 1) {
            auto tableName = toUppercase(words[1]);
            if (getTables().find(tableName) == getTables().end()) {
                fmt::println("Table '{}' does not exist.", tableName);
                return;
            }
            std::string folderPath;
            fmt::print("Enter the folder path where you want to save the file: ");
            std::getline(std::cin, folderPath);
            saveTableToFile(tableName, folderPath);
        fmt::println("Table {} saved to folder {} successfully.", tableName, folderPath);
        }
        //load table
    else if (words[0] == "load" && words.size() > 1) {
        auto tableName = toUppercase(words[1]);
        std::string folderPath;
        fmt::print("Enter the relative folder path from where you want to load the file: ");
        std::getline(std::cin, folderPath);

        try {
            loadTableFromFile(tableName, folderPath);
        }catch (const std::exception& e) {
            fmt::println("Error loading table: {}", e.what());
        }
    }  else {
            fmt::println("Invalid command. Please enter a valid command.");
        }
    }

//Menu function
auto Menu()->void { //https://cplusplus.com/reference/sstream/ - documentation to <sstream>
    fmt::println("Welcome to the Database Management System");
    Database db;
    std::regex spaces("\\s+"); // Regular expression to match multiple spaces

    while (true) {
        fmt::println("{}","Enter Command ('help' for list of commands or 'exit' to quit (Note: commands and data types are case-insensitive )): ");
        std::string command;
        std::getline(std::cin, command);
        command = std::regex_replace(command, spaces," "); // Replaces multiple spaces with a single space to make it easier for the user to enter commands

        // Pass the command to the executeCommand function
        db.executeCommand(command);
    }
}




















