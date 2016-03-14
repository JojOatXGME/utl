#include <list>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "utl/arguments.h"

using std::string;
using utl::Arguments;
using utl::argr::boolean;
using utl::argr::list;


utl::argr::withUnit<long> testUnit{
		{"k",  1000},
		{"M",  1000 * 1000},
		{"G",  1000 * 1000 * 1000},
		{"ki", 1024},
		{"Mi", 1024 * 1024},
		{"Gi", 1024 * 1024 * 1024}
};


// ---------------------------------------------------------------------------
// integer

TEST(ArgumentsParserTest, integer0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(     0, arg);
}

TEST(ArgumentsParserTest, integerMinus1)
{
	const char *argv[] = {"./myapp", "-1"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(    -1, arg);
}

TEST(ArgumentsParserTest, integer50)
{
	const char *argv[] = {"./myapp", "50"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(    50, arg);
}

TEST(ArgumentsParserTest, integerFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, integerFailSuffix)
{
	const char *argv[] = {"./myapp", "42Suffix"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, integerFailPrefix)
{
	const char *argv[] = {"./myapp", "Pre24"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

// ---------------------------------------------------------------------------
// double

TEST(ArgumentsParserTest, double00)
{
	const char *argv[] = {"./myapp", "0.0"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(   0.0, arg);
}

TEST(ArgumentsParserTest, double01)
{
	const char *argv[] = {"./myapp", "0.1"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(   0.1, arg);
}

TEST(ArgumentsParserTest, doubleMinus11)
{
	const char *argv[] = {"./myapp", "-1.1"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(  -1.1, arg);
}

TEST(ArgumentsParserTest, double500)
{
	const char *argv[] = {"./myapp", "50."};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg));
	EXPECT_EQ(  50.0, arg);
}

TEST(ArgumentsParserTest, doubleFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailSuffix)
{
	const char *argv[] = {"./myapp", "42+"};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailPrefix)
{
	const char *argv[] = {"./myapp", "#24"};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

TEST(ArgumentsParserTest, doubleFailDot)
{
	const char *argv[] = {"./myapp", "."};
	Arguments args(2, argv);

	double arg;
	EXPECT_THROW(     args.getNextArgument(arg), std::exception);
}

// ---------------------------------------------------------------------------
// unit

TEST(ArgumentsParserTest, unitInteger0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	int arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ(     0, arg);
}

TEST(ArgumentsParserTest, unitFloatMinus1)
{
	const char *argv[] = {"./myapp", "-1Mi"};
	Arguments args(2, argv);

	float arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ(-1048576, arg);
}

TEST(ArgumentsParserTest, unitDouble50)
{
	const char *argv[] = {"./myapp", "50k"};
	Arguments args(2, argv);

	double arg;
	EXPECT_TRUE(      args.getNextArgument(arg, testUnit));
	EXPECT_EQ( 50000, arg);
}

TEST(ArgumentsParserTest, unitFailEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, unitFailEmptyUnit)
{
	const char *argv[] = {"./myapp", "1"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, unitFailPrefix)
{
	const char *argv[] = {"./myapp", "P42"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

TEST(ArgumentsParserTest, integerFailNotFound)
{
	const char *argv[] = {"./myapp", "24L"};
	Arguments args(2, argv);

	int arg;
	EXPECT_THROW(     args.getNextArgument(arg, testUnit), std::exception);
}

// ---------------------------------------------------------------------------
// boolean

TEST(ArgumentsParserTest, boolean0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ( false, arg);
}

TEST(ArgumentsParserTest, boolean1)
{
	const char *argv[] = {"./myapp", "1"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ(  true, arg);
}

TEST(ArgumentsParserTest, booleanTrue)
{
	const char *argv[] = {"./myapp", "true"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ(  true, arg);
}

TEST(ArgumentsParserTest, booleanFalse)
{
	const char *argv[] = {"./myapp", "false"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ( false, arg);
}

TEST(ArgumentsParserTest, booleanOn)
{
	const char *argv[] = {"./myapp", "on"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ(  true, arg);
}

TEST(ArgumentsParserTest, booleanOff)
{
	const char *argv[] = {"./myapp", "off"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ( false, arg);
}

TEST(ArgumentsParserTest, booleanCase)
{
	const char *argv[] = {"./myapp", "TRUE", "FAlSE", "On", "oFf"};
	Arguments args(5, argv);

	bool arg;
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ(  true, arg);
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ( false, arg);
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ(  true, arg);
	EXPECT_TRUE(      args.getNextArgument(arg, boolean()));
	EXPECT_EQ( false, arg);
}

TEST(ArgumentsParserTest, booleanFailPrefix)
{
	const char *argv[] = {"./myapp", "x1"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_THROW(     args.getNextArgument(arg, boolean()), std::exception);
}

TEST(ArgumentsParserTest, booleanFailSuffix)
{
	const char *argv[] = {"./myapp", "0t"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_THROW(     args.getNextArgument(arg, boolean()), std::exception);
}

TEST(ArgumentsParserTest, booleanFail00)
{
	const char *argv[] = {"./myapp", "0.0"};
	Arguments args(2, argv);

	bool arg;
	EXPECT_THROW(     args.getNextArgument(arg, boolean()), std::exception);
}

// ---------------------------------------------------------------------------
// list

TEST(ArgumentsParserTest, list555)
{
	const char *argv[] = {"./myapp", "5,5,5"};
	Arguments args(2, argv);

	std::vector<int> arg;
	std::vector<int> expected = {5, 5, 5};
	EXPECT_TRUE(      args.getNextArgument(arg, list()));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, list0)
{
	const char *argv[] = {"./myapp", "0"};
	Arguments args(2, argv);

	std::list<int> arg;
	std::list<int> expected = {0};
	EXPECT_TRUE(      args.getNextArgument(arg, list()));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, listEmpty)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	std::vector<int> arg;
	std::vector<int> expected = {};
	EXPECT_TRUE(      args.getNextArgument(arg, list()));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, listSet)
{
	const char *argv[] = {"./myapp", "1,2,3,4,7"};
	Arguments args(2, argv);

	std::set<int> arg;
	std::set<int> expected = {1, 2, 3, 4, 7};
	EXPECT_TRUE(      args.getNextArgument(arg, list()));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, listCustomReader)
{
	const char *argv[] = {"./myapp", "true,false,off"};
	Arguments args(2, argv);

	std::vector<bool> arg;
	std::vector<bool> expected = {true, false, false};
	EXPECT_TRUE(      args.getNextArgument(arg, list(boolean())));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, listCustomDelimeter)
{
	const char *argv[] = {"./myapp", "0:0:1"};
	Arguments args(2, argv);

	std::vector<bool> arg;
	std::vector<bool> expected = {false, false, true};
	EXPECT_TRUE(      args.getNextArgument(arg, list(boolean(), ':')));
	EXPECT_EQ(expected, arg);
}

TEST(ArgumentsParserTest, listFail1)
{
	const char *argv[] = {"./myapp", "6,9,f"};
	Arguments args(2, argv);

	std::vector<int> arg;
	EXPECT_THROW(     args.getNextArgument(arg, list()), std::exception);
}

TEST(ArgumentsParserTest, listFail2)
{
	const char *argv[] = {"./myapp", "f,9,0"};
	Arguments args(2, argv);

	std::vector<int> arg;
	EXPECT_THROW(     args.getNextArgument(arg, list()), std::exception);
}

TEST(ArgumentsParserTest, listFail3)
{
	const char *argv[] = {"./myapp", "6,f,0"};
	Arguments args(2, argv);

	std::vector<int> arg;
	EXPECT_THROW(     args.getNextArgument(arg, list()), std::exception);
}
