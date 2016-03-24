use lib './lib';
use strict;
use warnings;
use JavaScript::Duktape;
use Data::Dumper;
use Test::More;
use Test::Fatal;

subtest 'rewrap func with nested call' => sub{
    my $js = JavaScript::Duktape->new();
    $js->set( inc => sub{
        return 1 + shift();
    });
    $js->set( each => sub{
        my $arr = shift;
        my $cb = shift;
        for(@$arr) {
            $cb->($_);
        }
    });
    my $ret = $js->eval(q{
        var x=0; each([11,22], function(i){ x+=i; x=inc(x); }); x
    });
    is $ret, 35,
};

subtest 'trap callback error' => sub{
    my $js = JavaScript::Duktape->new();
    $js->set( each => sub{
        die "foo here";
    });
    like exception { $js->eval(q{
        var x=0; each([11,22], function(i){ x+=i });
    }) }, qr/foo here/;
};

subtest 'trap rewrap func error within nested call' => sub{
    my $js = JavaScript::Duktape->new();
    $js->set( inc => sub{
        die "bar here";
    });
    $js->set( each => sub{
        my $arr = shift;
        my $cb = shift;
        for(@$arr) {
            $cb->($_);
        }
    });
    like exception { $js->eval(q{
        var x=0; each([11,22], function(i){ x+=i; x=inc(x); });
    }) }, qr/bar here/;
};

done_testing;
