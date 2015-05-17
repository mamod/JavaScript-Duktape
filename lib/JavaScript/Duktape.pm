package JavaScript::Duktape;
use strict;
use warnings;
use Data::Dumper;
use Scalar::Util 'looks_like_number';

our $VERSION = '0.0.1_2';

use base qw/Exporter/;
our @EXPORT = qw (
    DUK_TYPE_NONE
    DUK_TYPE_UNDEFINED
    DUK_TYPE_NULL
    DUK_TYPE_BOOLEAN
    DUK_TYPE_NUMBER
    DUK_TYPE_STRING
    DUK_TYPE_OBJECT
    DUK_TYPE_BUFFER 
    DUK_TYPE_POINTER
    DUK_TYPE_LIGHTFUNC
    DUK_ENUM_INCLUDE_NONENUMERABLE
    DUK_ENUM_INCLUDE_INTERNAL
    DUK_ENUM_OWN_PROPERTIES_ONLY
    DUK_ENUM_ARRAY_INDICES_ONLY
    DUK_ENUM_SORT_ARRAY_INDICES
    DUK_ENUM_NO_PROXY_BEHAVIOR
    DUK_TYPE_MASK_NONE
    DUK_TYPE_MASK_UNDEFINED
    DUK_TYPE_MASK_NULL
    DUK_TYPE_MASK_BOOLEAN
    DUK_TYPE_MASK_NUMBER
    DUK_TYPE_MASK_STRING
    DUK_TYPE_MASK_OBJECT
    DUK_TYPE_MASK_BUFFER
    DUK_TYPE_MASK_POINTER
    DUK_TYPE_MASK_LIGHTFUNC
    DUK_TYPE_MASK_THROW
    DUK_COMPILE_EVAL
    DUK_COMPILE_FUNCTION
    DUK_COMPILE_STRICT
    DUK_COMPILE_SAFE
    DUK_COMPILE_NORESULT
    DUK_COMPILE_NOSOURCE
    DUK_COMPILE_STRLEN
    DUK_DEFPROP_WRITABLE
    DUK_DEFPROP_ENUMERABLE
    DUK_DEFPROP_CONFIGURABLE
    DUK_DEFPROP_HAVE_WRITABLE
    DUK_DEFPROP_HAVE_ENUMERABLE
    DUK_DEFPROP_HAVE_CONFIGURABLE
    DUK_DEFPROP_HAVE_VALUE
    DUK_DEFPROP_HAVE_GETTER
    DUK_DEFPROP_HAVE_SETTER
    DUK_DEFPROP_FORCE
    DUK_VARARGS
    null
    true
    false
);

##constants
use constant {
    DUK_TYPE_NONE      => 0,
    DUK_TYPE_UNDEFINED => 1,
    DUK_TYPE_NULL      => 2,
    DUK_TYPE_BOOLEAN   => 3,
    DUK_TYPE_NUMBER    => 4,
    DUK_TYPE_STRING    => 5,
    DUK_TYPE_OBJECT    => 6,
    DUK_TYPE_BUFFER    => 7,
    DUK_TYPE_POINTER   => 8,
    DUK_TYPE_LIGHTFUNC => 9,

    DUK_TYPE_MASK_NONE       => (1 << 0),
    DUK_TYPE_MASK_UNDEFINED  => (1 << 1),
    DUK_TYPE_MASK_NULL       => (1 << 2),
    DUK_TYPE_MASK_BOOLEAN    => (1 << 3),
    DUK_TYPE_MASK_NUMBER     => (1 << 4),
    DUK_TYPE_MASK_STRING     => (1 << 5),
    DUK_TYPE_MASK_OBJECT     => (1 << 6),
    DUK_TYPE_MASK_BUFFER     => (1 << 7),
    DUK_TYPE_MASK_POINTER    => (1 << 8),
    DUK_TYPE_MASK_LIGHTFUNC  => (1 << 9),
    DUK_TYPE_MASK_THROW      => (1 << 10),

    # Enumeration flags for duk_enum()
    DUK_ENUM_INCLUDE_NONENUMERABLE => (1 << 0),
    DUK_ENUM_INCLUDE_INTERNAL      => (1 << 1),
    DUK_ENUM_OWN_PROPERTIES_ONLY   => (1 << 2),
    DUK_ENUM_ARRAY_INDICES_ONLY    => (1 << 3),
    DUK_ENUM_SORT_ARRAY_INDICES    => (1 << 4),
    DUK_ENUM_NO_PROXY_BEHAVIOR     => (1 << 5),

    DUK_COMPILE_EVAL               => (1 << 0),   
    DUK_COMPILE_FUNCTION           => (1 << 1),
    DUK_COMPILE_STRICT             => (1 << 2),
    DUK_COMPILE_SAFE               => (1 << 3),
    DUK_COMPILE_NORESULT           => (1 << 4),
    DUK_COMPILE_NOSOURCE           => (1 << 5),
    DUK_COMPILE_STRLEN             => (1 << 6),

    #Flags for duk_def_prop() and its variants
    DUK_DEFPROP_WRITABLE           => (1 << 0),
    DUK_DEFPROP_ENUMERABLE         => (1 << 1),
    DUK_DEFPROP_CONFIGURABLE       => (1 << 2),
    DUK_DEFPROP_HAVE_WRITABLE      => (1 << 3),
    DUK_DEFPROP_HAVE_ENUMERABLE    => (1 << 4),
    DUK_DEFPROP_HAVE_CONFIGURABLE  => (1 << 5),
    DUK_DEFPROP_HAVE_VALUE         => (1 << 6),
    DUK_DEFPROP_HAVE_GETTER        => (1 << 7),
    DUK_DEFPROP_HAVE_SETTER        => (1 << 8),
    DUK_DEFPROP_FORCE              => (1 << 9),
    DUK_VARARGS                    => -1
};

sub new {
    my $class= shift;
    my $self = bless {}, $class;
    $self->{duk} = JavaScript::Duktape::Vm->new();
    $self->{func} = {};
    return $self;
}

sub null  { $JavaScript::Duktape::Data::null; }
sub true  { $JavaScript::Duktape::Bool::true; }
sub false { $JavaScript::Duktape::Bool::false }

sub set {
    my $self = shift;
    my $name = shift;
    my $val = shift;
    my $vm = $self->vm;
    $vm->push_perl($val);
    $vm->put_global_string($name);
    return 1;
}

sub get {
    my $self = shift;
    my $name = shift;
    my $vm = $self->vm;
    $vm->push_string($name);
    $vm->eval();
    my $ret = $vm->to_perl(-1);
    $vm->pop();
    return $ret;
}

sub vm { shift->{duk}; }
sub duk { shift->{duk}; }

sub DESTROY {
    my $self = shift;
    my $duk = $self->duk;
    $duk->destroy_heap();
}

=head1 NAME

JavaScript::Duktape - Perl interface to Duktape embeddable javascript engine

=for html
<a href="https://travis-ci.org/mamod/JavaScript-Duktape"><img src="https://travis-ci.org/mamod/JavaScript-Duktape.svg?branch=master"></a>

=head1 SYNOPSIS
    
    use JavaScript::Duktape;

    ##create new js context
    my $js = JavaScript::Duktape->new();

    #set function to be used from javascript land
    $js->set('write' => sub {
        print $_[0], "\n";
    });

    $js->eval(qq~
        (function(){
            for (var i = 0; i < 100; i++){
                write(i);
            }
        })();
    ~);
=cut

package JavaScript::Duktape::Vm;
use strict;
use warnings;
no warnings 'redefine';
use Scalar::Util 'looks_like_number';
use Data::Dumper;
use Config;
use JavaScript::Duktape::C::libPath;

my $Duklib;

BEGIN {
    my $FunctionsMap = JavaScript::Duktape::C::libPath::getPath("FunctionsMap.pl");
    require $FunctionsMap;

    $Duklib = $^O eq 'MSWin32' ?
                JavaScript::Duktape::C::libPath::getPath('duktape.dll') :
                JavaScript::Duktape::C::libPath::getPath('duktape.so');
};

use Inline C => config =>
    typemaps => JavaScript::Duktape::C::libPath::getPath('typemap'),
    INC => '-I' . JavaScript::Duktape::C::libPath::getPath('../C');

#XXX : build duktape as a shared library and link it
# use Inline C => config =>
#     myextlib => $Duklib,
#     LIBS => '-L'. JavaScript::Duktape::C::libPath::getPath('../C') . ' -lduktape';

use Inline C => JavaScript::Duktape::C::libPath::getPath('duktape_wrap.c');

my $SUB = 0;
my $Functions = {};



sub push_perl {
    my $self = shift;
    my $val = shift;

    my $ref = ref $val;
    if ($ref){
        if ($ref eq 'JavaScript::Duktape::Data'){
            $self->push_null();
        } elsif ($ref eq 'JavaScript::Duktape::Bool'){
            if ($val){
                $self->push_true();
            } else {
                $self->push_false();
            }
        } elsif ($ref eq 'ARRAY'){
            my $arr_idx = $self->push_array();
            my $len = scalar @{$val};
            for (my $idx = 0; $idx < $len; $idx++) {
                $self->push_perl($val->[$idx]);
                $self->put_prop_index($arr_idx, $idx);
            }
        } elsif ($ref eq 'HASH'){
            $self->push_object();
            while (my ($k, $v) = each %{$val}) {
                $self->push_string($k);
                $self->push_perl($v);
                $self->put_prop(-3);
            }
        } elsif ($ref eq 'CODE') {
            $self->push_function( sub {
                my $top = $self->get_top();
                my @args = ($self);
                for (my $i = 0; $i < $top; $i++){
                    push @args, $self->to_perl($i);
                }
                my $ret = $val->(@args);
                $self->push_perl($ret);
                return 1;
            });
        }
    } else {
        if (!defined $val){
            $self->push_undefined();
        } elsif (looks_like_number( $val )){
            $self->push_number($val);
        } else {
            $self->push_string($val);
        }
    }
}

sub to_perl {
    my $self = shift;
    my $index = shift;
    my $ret;
    
    my $type = $self->get_type($index);

    if ($type == JavaScript::Duktape::DUK_TYPE_STRING){
        $ret = $self->get_string($index);
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_NUMBER){
        $ret = $self->get_number($index) + 0;
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_OBJECT){

        if ($self->is_function($index)){
            my $ptr = $self->get_heapptr($index);
            return sub {
                $self->push_heapptr($ptr);
                my $len = 0+@_;
                for (my $i = 0; $i < $len; $i++){
                    $self->push_perl($_[$i]);
                }
                $self->call($len);
                my $ret = $self->to_perl(-1);
                $self->pop();
                return $ret;
            };
        }

        my $isArray = $self->is_array($index);
        $ret = $isArray ? [] : {};
        $self->enum($index, JavaScript::Duktape::DUK_ENUM_OWN_PROPERTIES_ONLY);

        while ($self->next(-1, 1)) {
            my $key = $self->to_perl(-2);
            my $val = $self->to_perl(-1);
            
            $self->pop_n(2);

            if ($isArray) {
                $ret->[$key] = $val;
            } else {
                $ret->{$key} = $val;
            }
        }
        $self->pop();
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_BOOLEAN){
        my $bool =  $self->get_boolean($index);
        if ($bool == 1){
            $ret = JavaScript::Duktape::Bool::true();
        } else {
            $ret = JavaScript::Duktape::Bool::false();
        }
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_NULL){
        $ret = JavaScript::Duktape::Data::null();
    }

    ##XXX to do
    elsif ($type == JavaScript::Duktape::DUK_TYPE_POINTER){
        $ret = "pointer";
    }

    else {
        $ret = undef;
    }
    
    return $ret;
}

=head1 VM API

vm api corresponds to Duktape Engine API see L<http://duktape.org/api.html>
To access vm create new context then call C<vm>

    my $js = JavaScript::Duktape->new();
    my $duk = $js->vm;

    #now you can call Duktape API from perl 

    $duk->push_string('print');
    $duk->eval();
    $duk->push_string('hi');
    $duk->call(1);
    $duk->pop();

Also you may find it useful to use C<dump> function
regularly to get a better idea where you're in the stack, the following code is the same example
above but with using C<dump> function to get a glance of stack top

    my $js = JavaScript::Duktape->new();
    my $duk = $js->duk;

    #push "print" string
    $duk->push_string('print');
    $duk->dump(); #-> [ Duktape (top=1): print ]
    
    #since print is a native function we need to evaluate it
    $duk->eval();
    $duk->dump(); #-> [ Duktape (top=1): function print() {/* native */} ]

    #push one argument to print function
    $duk->push_string('hi');
    $duk->dump(); #-> [ Duktape (top=2): function print() {/* native */} hi ]
    
    #now call print function and pass "hi" as one argument
    $duk->call(1);

    #since print function doesn't return any value, it will push undefined to the stack
    $duk->dump(); #-> [ Duktape (top=1): undefined ]

    #pop to remove undefined from stack top
    $duk->pop();

    #Bingo
    $duk->dump(); #-> [ Duktape (top=0): ]

=cut

##############################################
# push functions
##############################################
sub push_function {
    my $self = shift;
    my $sub = shift;
    my $nargs = shift;
    if (!defined $nargs){ $nargs = -1 }

    $SUB++;
    $Functions->{$SUB} = sub {
        my $top = $self->get_top();
        my $ret = 1;
        $self->perl_duk_safe_call(sub {
            eval { $ret = $sub->($self) };
            my $error = $@;
            if ($error){
                if ($error =~ /^Duk::Error/){
                    die $@;
                } else {
                    $error =~ s/\n//g;
                    $error =~ s/\\/\\\\/g;
                    $self->eval_string("throw new Error('$error')");
                }
            }
            return $ret;
        }, $top, 1);
        return $ret;
    };

    $self->perl_push_function($Functions->{$SUB}, $nargs);
}

#####################################################################
# safe call
#####################################################################
sub safe_call {
    my $self = shift;
    my $sub = shift;
    my $safe = sub {
        return $sub->($self);
    };

    my $oldtop = $self->get_top();
    eval { $self->perl_duk_safe_call($safe, @_) };
    my $error = $@;
    if ($error) {
        if ($error =~ /^Duk::Error/i) {
            # error came from duktape do nothing;
        } else {
            #error from perl push error object
            #and clear previous pushed stack
            my $newtop = $self->get_top();
            if ($newtop > $oldtop){
                $self->pop_n($newtop - $oldtop);
            }
            $error =~ s/\n//g;
            $error =~ s/\\/\\\\/g;
            $self->reset_top();
            $self->eval_string("var t = new Error('$error'); t;");
        }
        return 1;
    }
    return 0;
}

##############################################
# overridden functions
##############################################
*require_context = \&perl_duk_require_context;


##############################################
# helper functions
##############################################
*reset_top = \&perl_duk_reset_top;

sub dump {
    my $self = shift;
    my $name = shift || "Duktape";
    my $fh  = shift || \*STDOUT;
    my $n = $self->get_top();
    printf $fh "%s (top=%ld):", $name, $n;
    for (my $i = 0; $i < $n; $i++) {
        printf $fh " ";
        $self->dup($i);
        printf $fh "%s", $self->safe_to_string(-1);
        $self->pop();
    }
    printf $fh "\n";
}

sub DESTROY {}

package JavaScript::Duktape::Bool; {
    use warnings;
    use strict;
    our ($true, $false);
    use overload
        '""' => sub { ${$_[0]} },
        'bool' => sub { ${$_[0]} ? 1 : 0 },
        fallback => 1;

    BEGIN {
        my $t = 1;
        my $f = 0;
        $true  = bless \$t, 'JavaScript::Duktape::Bool';
        $false = bless \$f, 'JavaScript::Duktape::Bool';
    }

    sub true  { $true }
    sub false { $false }
}

package JavaScript::Duktape::Data; {
    use warnings;
    use strict;
    our ($null);
    use overload
        '""' => sub { ${$_[0]} },
        'bool' => sub { ${$_[0]} ? 1 : 0 },
        fallback => 1;

    BEGIN {
        my $n = '';
        $null  = bless \$n, 'JavaScript::Duktape::Data';
    }

    sub null  { $null }
}

1;
