use lib './lib';
use strict;
use warnings;
use JavaScript::Duktape;
use Data::Dumper;

require './t/helper.pl';


my $NONEXISTENT_FILE = '/this/file/doesnt/exist';

my $js = JavaScript::Duktape->new();
my $duk = $js->duk;

sub my_func {
	# die;
	$duk->push_current_function();
	$duk->get_prop_string(-1, "name");
	printf("my name is: '%s'\n", $duk->safe_to_string(-1));
	$duk->pop_2();

	return -106;
}

sub test_without_name {
	$duk->push_c_function(\&my_func, 0);
	$duk->put_global_string("MyFunc");

	$duk->eval_string_noresult(
		"try {\n"
		. "    [1].forEach(MyFunc);\n"
		. "} catch (e) {\n"
		. "    print(sanitize(e.stack || e));\n"
		. "}\n"
	);

	return 0;
}

sub test_with_name {
	$duk->get_global_string("MyFunc");
	$duk->push_string("my_func");
	$duk->put_prop_string(-2, "name");
	$duk->pop();

	$duk->eval_string_noresult(
		"try {\n"
		. "    [1].forEach(MyFunc);\n"
		. "} catch (e) {\n"
		. "    print(sanitize(e.stack || e));\n"
		. "}\n"
	);

	return 0;
}

$duk->eval_string_noresult(
	"var sanitize = function(v) {\n"
	. "    v = v.replace(/eval \\S+/, 'eval XXX');\n"
	. "    return v;\n"
	. "}\n"
);

TEST_SAFE_CALL($duk, \&test_without_name, 'test_without_name');
TEST_SAFE_CALL($duk, \&test_with_name, 'test_with_name');

test_stdout();

__DATA__
*** test_without_name (duk_safe_call)
my name is: ''
URIError: uri error (rc -106)
    at [anon] () native strict preventsyield
    at forEach () native strict preventsyield
    at eval XXX preventsyield
==> rc=0, result='undefined'
*** test_with_name (duk_safe_call)
my name is: 'my_func'
URIError: uri error (rc -106)
    at my_func () native strict preventsyield
    at forEach () native strict preventsyield
    at eval XXX preventsyield
==> rc=0, result='undefined'
