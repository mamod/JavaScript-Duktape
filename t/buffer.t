use strict;
use warnings;
use Data::Dumper;
use lib './lib';
use JavaScript::Duktape;
use Test::More;

my $js = JavaScript::Duktape->new();
my $duk = $js->duk;

$js->set('process', {});
$js->set('process.test', {});

$js->set('testPerlBuffer', sub {
    my ($duk, $buf, $a, $b, $c) = @_;
    is ref $buf, 'JavaScript::Duktape::Buffer';
    is $a, 97;
    is $b, 98;
    is $c, 99;
});

$js->eval(qq~
    function testBuffer(buf){
        testPerlBuffer(buf, buf[0], buf[1], buf[2]);
        return buf;
    }
    1;
~);

$duk->push_string('testBuffer');
$duk->eval();
$duk->push_string('abc');
my $len;
$duk->to_buffer(-1, $len);
$duk->call(1);

my $buf = $duk->to_perl(-1);
is ref $buf, 'JavaScript::Duktape::Buffer';
is $buf->toString(), 'abc';
is $buf->(0), 97;
is $buf->(1), 98;
is $buf->(2), 99;

done_testing();
