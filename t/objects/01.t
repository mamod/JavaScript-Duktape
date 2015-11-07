use strict;
use warnings;
use Data::Dumper;
use lib './lib';
use JavaScript::Duktape;
use Test::More;

my $js = JavaScript::Duktape->new();
my $duk = $js->duk;

$duk->eval_string(qq~
    var tt = {};
    tt.all = test;
    function test (fn){
        this.name       = 'Mamod';
        this.lastname   = "Foo";
        this.counter    = 0;
        this.test       = function(){
            this.counter++;
            fn();
        };
    }

    test.prototype.setLast = function(fn){
        this.lastname = fn;
        this.fullname = this.name + " " + fn('Mehy');
    }
    test;
~);

my $obj = $duk->to_perl_object(-1);
$duk->pop();

for (0 .. 100){
    my $t = $obj->new(sub{});
    $t->setLast( sub {
        my $this = shift;
        my $last = shift . "ar";
        return $last;
    });
    is $t->lastname('Mehy'), "Mehyar";
    is $t->fullname, "Mamod Mehyar";
}

my $t = $obj->new(sub {
    my $this = shift;
    ok(1);
});

$t->setLast( sub {
    my $this = shift;
    my $last = shift . "ar";
    $this->test("");
    return $last;
});
is $t->lastname('Mehy2'), "Mehy2ar";
is $t->fullname, "Mamod Mehyar";
is $t->counter, 2;

done_testing(207);
