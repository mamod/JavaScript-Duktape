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



# { ##setting global functions

#     my $t = timeit($count, sub{
#         $js->set('test', sub{});
#     });
#     print_results("set global functions", $t);
# }

# { ##getting javascript object

#     my $t = timeit($count, sub{
#         $duk->eval_string(qq~
#             function javascriptObject1 (){}
#             javascriptObject1;
#         ~);

#         my $obj = $duk->to_perl_object(-1);
#         $duk->pop();
#     });
#     print_results("getting objects", $t);
# }

{ ##object operations
    $duk->eval_string(qq~
        function javascriptObject (){
            this.test = 9;
            this.callme = function(n){
                this.test = n;
            }
        }

        javascriptObject.prototype.setfunc = function(fn){
            this.fn = fn;
            print(this.fn(9,8));
        };

        javascriptObject.prototype.testcall = function(a,b){
            this.fn(a,9);
            return a+b;
        };

        javascriptObject;
    ~);
    my $obj = $duk->to_perl_object(-1);
    $duk->pop();

    # { #calling new on objects
    #     my $t = timeit($count, sub{
    #         my $o = $obj->new();
    #     });
    #     print_results("calling new", $t);
    # }

    # { #calling function from objects
    #     my $o = $obj->new();
    #     my $i = 0;
    #     my $t = timeit($count, sub{
    #         $o->callme($i++);
    #         # print $o->test, "\n";
    #     });
    #     print_results("calling function", $t);
    # }

    { #setting functions
        my $o = $obj->new();
        my $i = 0;

        $o->setfunc($duk->cache( sub {
            # print Dumper \@_;
            # print "cool\n";

            # print "cool\n";
            return 88;
        }));

        my $t = timeit($count, sub{
            # my $n = $o->fn($i++, 7);
            my $result = $o->testcall(1,3);
            # print $result, "\n";
        });
        print_results("calling cached function", $t);
    }
}

my @results;

sub print_results {
    my $name = shift;
    my $t    = shift;

    my @result = split('@', timestr($t));

    my $r = "";
    $r .= "===================================\n";
    $r .= "$count $name\n";
    $r .= "===================================\n";
    $r .= " " . $result[0] . "\n";
    $r .= $result[1] . "\n";
    $r .= "\n\n";

    push @results, $r;
}

for (@results){
    print $_;
}
