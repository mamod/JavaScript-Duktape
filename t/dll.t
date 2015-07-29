use lib './lib';
use strict;
use warnings;
use JavaScript::Duktape;
use JavaScript::Duktape::C::libPath;
use Data::Dumper;
use Carp;
use Test::More;

my $js = JavaScript::Duktape->new();
my $duk = $js->duk;

my $duktapePath = JavaScript::Duktape::C::libPath::getPath('../C');
my $duktape = JavaScript::Duktape::C::libPath::getPath('duktape.c');

my $build = "gcc -Wall -I$duktapePath $duktape ./t/dll.c -shared -o dll.shared";
system($build) && plan skip_all => "Can't compile shared library using gcc compiler";

# print Dumper $build;

my $dll = $duk->duktape_dlOpen("./dll.shared");
my $fn = $duk->to_perl(-1);

my $ret = $fn->{add_number}->(5,5);

is ($ret, 10);


ok $duk->duktape_dlClose($dll);
unlink 'dll.shared' or fail $!;

done_testing();
