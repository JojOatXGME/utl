#include <string>

#include <gtest/gtest.h>

#include "utl/arguments.h"

using std::string;
using utl::Arguments;


TEST(ArgumentsTest, empty)
{
	const char *argv[] = {"./myapp"};
	Arguments args(1, argv);

	string param;
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, onlyArgument)
{
	const char *argv[] = {"./myapp", "test"};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(    "test", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, onlyShortOption)
{
	const char *argv[] = {"./myapp", "-x"};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, onlyShortOptionCombi)
{
	const char *argv[] = {"./myapp", "-xy"};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ(      "-y", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, onlyLongOption)
{
	const char *argv[] = {"./myapp", "--test"};
	Arguments args(2, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, emptyArgument)
{
	const char *argv[] = {"./myapp", ""};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(        "", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, emptyParameter)
{
	const char *argv[] = {"./myapp", "-x", ""};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(        "", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, shortOptionAndArgument)
{
	const char *argv[] = {"./myapp", "-x", "test"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(    "test", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionAndArgument)
{
	const char *argv[] = {"./myapp", "--test", "arg"};
	Arguments args(3, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, shortOptionBehind)
{
	const char *argv[] = {"./myapp", "test", "-x"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(    "test", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionBehind)
{
	const char *argv[] = {"./myapp", "arg", "--test"};
	Arguments args(3, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, shortOptionWithParam1)
{
	const char *argv[] = {"./myapp", "-xparam"};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, shortOptionWithParam2)
{
	const char *argv[] = {"./myapp", "-x", "param"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, shortOptionWithMissigParam1)
{
	const char *argv[] = {"./myapp", "arg", "-x"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, shortOptionWithMissigParam2)
{
	const char *argv[] = {"./myapp", "arg", "-xparam1"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "param1", param);
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, shortOptionWithMissigParam3)
{
	const char *argv[] = {"./myapp", "arg", "-x", "param1"};
	Arguments args(4, argv);

	string param;
	EXPECT_EQ(         3, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "param1", param);
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, longOptionWithParam1)
{
	const char *argv[] = {"./myapp", "--test=param"};
	Arguments args(2, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionWithParam2)
{
	const char *argv[] = {"./myapp", "--test", "param"};
	Arguments args(3, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionInvalidParam)
{
	const char *argv[] = {"./myapp", "--test=param"};
	Arguments args(2, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(        -1, args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionParamDistinction)
{
	const char *argv[] = {"./myapp", "--test", "param"};
	Arguments args(3, argv);
	args.registerOption("--test", 'x');
	args.registerOption("--test=", 'y');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, minusAsArgument)
{
	const char *argv[] = {"./myapp", "-"};
	Arguments args(2, argv);

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(       "-", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, shortOptionAsArgument)
{
	const char *argv[] = {"./myapp", "--", "-x"};
	Arguments args(3, argv);

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(      "-x", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, longOptionAsArgument)
{
	const char *argv[] = {"./myapp", "--", "--test"};
	Arguments args(3, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "--test", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
}

TEST(ArgumentsTest, shortOptionRemap)
{
	const char *argv[] = {"./myapp", "-x"};
	Arguments args(2, argv);
	args.registerOption("-x", 'y');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionIsPrefix)
{
	const char *argv[] = {"./myapp", "--test"};
	Arguments args(2, argv);
	args.registerOption("--test",  'x');
	args.registerOption("--test2", 'y');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionExpand)
{
	const char *argv[] = {"./myapp", "--te"};
	Arguments args(2, argv);
	args.registerOption("--test", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionExpandWithParam1)
{
	const char *argv[] = {"./myapp", "--te=param"};
	Arguments args(2, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionExpandWithParam2)
{
	const char *argv[] = {"./myapp", "--te", "param"};
	Arguments args(3, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionAmbiguous)
{
	const char *argv[] = {"./myapp", "--test"};
	Arguments args(2, argv);
	args.registerOption("--test1", 'x');
	args.registerOption("--test2", 'y');

	std::map<string,int> expected {
		std::make_pair("--test1", 'x'),
		std::make_pair("--test2", 'y')
	};

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(        -2, args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(  expected, args.getPossibleOptions());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionAmbiguousWithParam1)
{
	const char *argv[] = {"./myapp", "--test=param"};
	Arguments args(2, argv);
	args.registerOption("--test1=", 'x');
	args.registerOption("--test2=", 'y');

	std::map<string,int> expected {
		std::make_pair("--test1=", 'x'),
		std::make_pair("--test2=", 'y')
	};

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(        -2, args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(  expected, args.getPossibleOptions());
	EXPECT_TRUE(          args.hasParameter());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionAmbiguousWithParam2)
{
	const char *argv[] = {"./myapp", "--test", "param"};
	Arguments args(3, argv);
	args.registerOption("--test1=", 'x');
	args.registerOption("--test2=", 'y');

	std::map<string,int> expected {
		std::make_pair("--test1=", 'x'),
		std::make_pair("--test2=", 'y')
	};

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(        -2, args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(  expected, args.getPossibleOptions());
	EXPECT_FALSE(         args.hasParameter());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
}

TEST(ArgumentsTest, longOptionParamCheckTrue)
{
	const char *argv[] = {"./myapp", "--test=param"};
	Arguments args(2, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.hasParameter());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, longOptionParamCheckFalse)
{
	const char *argv[] = {"./myapp", "--test", "param"};
	Arguments args(3, argv);
	args.registerOption("--test=", 'x');

	string param;
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_FALSE(         args.hasParameter());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(   "param", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, strictSuccess)
{
	const char *argv[] = {"./myapp", "-x", "--test", "arg"};
	Arguments args(4, argv, true);
	args.registerOption( "-x",    'x');
	args.registerOption("--test", 'y');

	string param;
	EXPECT_EQ(         3, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, strictFailure)
{
	const char *argv[] = {"./myapp", "-x", "--test", "arg"};
	Arguments args(4, argv, true);
	args.registerOption("--test", 'y');

	string param;
	EXPECT_EQ(         3, args.getArgumentsLeft());
	EXPECT_EQ(        -1, args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(     "arg", param);
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}

TEST(ArgumentsTest, mix)
{
	const char *argv[] = {"./myapp", "-xyparam1", "--test=param2", "arg1",
				"--test", "arg2", "--test2", "param3", "--t"};
	Arguments args(9, argv);
	args.registerOption("--test",   'x');
	args.registerOption("--test=",  'y');
	args.registerOption("--test2=", 'z');

	std::map<string,int> expected {
		std::make_pair("--test",   'x'),
		std::make_pair("--test2=", 'z')
	};

	string param;
	// -xyparam1
	EXPECT_EQ(         8, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(      "-x", args.getOptionName());
	EXPECT_EQ(         8, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ(      "-y", args.getOptionName());
	EXPECT_EQ(         8, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "param1", param);
	// --test=param2
	EXPECT_EQ(         7, args.getArgumentsLeft());
	EXPECT_EQ(       'y', args.getNextOption());
	EXPECT_EQ( "--test=", args.getOptionName());
	EXPECT_EQ(         7, args.getArgumentsLeft());
	EXPECT_TRUE(          args.hasParameter());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "param2", param);
	// arg1 will be handled later
	// --test
	EXPECT_EQ(         6, args.getArgumentsLeft());
	EXPECT_EQ(       'x', args.getNextOption());
	EXPECT_EQ(  "--test", args.getOptionName());
	EXPECT_FALSE(         args.hasParameter());
	// arg2 will be handled later
	// --test2
	EXPECT_EQ(         5, args.getArgumentsLeft());
	EXPECT_EQ(       'z', args.getNextOption());
	EXPECT_EQ("--test2=", args.getOptionName());
	EXPECT_FALSE(         args.hasParameter());
	EXPECT_EQ(         4, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(  "param3", param);
	// --t
	EXPECT_EQ(         3, args.getArgumentsLeft());
	EXPECT_EQ(        -2, args.getNextOption());
	EXPECT_EQ(     "--t", args.getOptionName());
	EXPECT_EQ(  expected, args.getPossibleOptions());
	EXPECT_FALSE(         args.hasParameter());
	// no param for --t available
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	// all options handled
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_EQ(         0, args.getNextOption());
	//arg1
	EXPECT_EQ(         2, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(    "arg1", param);
	//arg2
	EXPECT_EQ(         1, args.getArgumentsLeft());
	EXPECT_TRUE(          args.getNextArgument(param));
	EXPECT_EQ(    "arg2", param);
	// end
	EXPECT_EQ(         0, args.getArgumentsLeft());
	EXPECT_FALSE(         args.getNextArgument(param));
	EXPECT_EQ(         0, args.getNextOption());
	EXPECT_EQ(         0, args.getArgumentsLeft());
}
