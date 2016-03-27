use lib './lib';
use strict;
use warnings;
use JavaScript::Duktape;
use Test::More;

use FindBin '$Bin';
my $ts_path = "$Bin/data/typescript.js";

subtest 'typescript compile' => sub {
    my $js = JavaScript::Duktape->new;

    $js->set( load=>sub{ _load_ts(); } );

    my $ts = $js->eval( q{
        Duktape.modSearch = function(){ return load() };
        var ts = require('ts');
        ts;
    } );

    is ref( $ts->{transpile} ), 'CODE';
};

subtest 'typescript transpile' => sub {
    my $js = JavaScript::Duktape->new;

    $js->set( load=>sub{ _load_ts(); } );

    my $code = $js->eval( q{
        Duktape.modSearch = function(){ return load() };
        var ts = require('ts');
        var code = ts.transpile('class Foo { bar: string; constructor(){ this.bar="baz" }; };');
        code;
    } );

    like $code, qr/Foo.*function/;

    my $ret = $js->eval( $code . "; (new Foo()).bar" );

    is $ret, 'baz';
};

done_testing;

sub _load_ts {
    local $/;
    open my $ff,'<', $ts_path;
    <$ff>;
}
