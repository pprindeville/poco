//
// RecordSet.cpp
//
// $Id: //poco/Main/Data/samples/RecordSet/src/RecordSet.cpp#2 $
//
// This sample demonstrates the Data library.
//
/// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>


using namespace Poco::Data::Keywords;
using Poco::DateTime;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;


struct Person
{
	std::string name;
	std::string address;
	int         age;
};


int main(int argc, char** argv)
{
	// register SQLite connector
	Poco::Data::SQLite::Connector::registerConnector();
	
	// create a session
	Session session("SQLite", "sample.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS Person", now;
	
	// (re)create table
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3), Birthday DATE)", now;
	
	// insert some rows
	session << "INSERT INTO Person VALUES('Bart Simpson', 'Springfield', 12, ?)", use(DateTime(1980, 4, 1)), now;
	session << "INSERT INTO Person VALUES('Lisa Simpson', 'Springfield', 10, ?)", use(DateTime(1982, 5, 9)), now;
		
	// a simple query
	Statement select(session);
	select << "SELECT * FROM Person";
	select.execute();

	// create a RecordSet 
	RecordSet rs(select);
	std::size_t cols = rs.columnCount();
	// print all column names
	for (std::size_t col = 0; col < cols; ++col)
	{
		std::cout << rs.columnName(col) << std::endl;
	}
	// iterate over all rows and columns
	bool more = rs.moveFirst();
	while (more)
	{
		for (std::size_t col = 0; col < cols; ++col)
		{
			std::cout << rs[col].convert<std::string>() << " ";
		}
		std::cout << std::endl;
		more = rs.moveNext();
	}

	return 0;
}
