use lib './lib';
use strict;
use warnings;
use JavaScript::Duktape;
use Data::Dumper;
use Benchmark qw(:all :hireswallclock) ;

my $js  = JavaScript::Duktape->new();
my $duk = $js->duk;

my $time = 5;
my $count = 25000;



{ ##settung global functions

    my $t = timeit($count, sub{
        $js->set('test', sub{});
    });
    print_results("set global functions", $t);
}

{ ##getting javascript object

    my $t = timeit($count, sub{
        $duk->eval_string(qq~
            function javascriptObject1 (){}
            javascriptObject1;
        ~);

        my $obj = $duk->to_perl_object(-1);
        $duk->pop();
    });
    print_results("getting objects", $t);
}

{ ##object operations
    $duk->eval_string(qq~
        function javascriptObject (){
            this.callme = function(n){
                this.test = n;
            }
        }
        javascriptObject;
    ~);
    my $obj = $duk->to_perl_object(-1);
    $duk->pop();

    { #calling new on objects
        my $t = timeit($count, sub{
            my $o = $obj->new();
        });
        print_results("calling new", $t);
    }

    { #calling function from objects
        my $o = $obj->new();
        my $i = 0;
        my $t = timeit($count, sub{
            $o->callme($i++);
            # print $o->test, "\n";
        });
        print_results("calling function", $t);
    }
}

sub print_results {
    my $name = shift;
    my $t    = shift;

    my @result = split('@', timestr($t));

    print "===================================\n";
    print "$count $name\n";
    print "===================================\n";
    print " " . $result[0], "\n";
    print $result[1], "\n";
    print "\n\n";
}
