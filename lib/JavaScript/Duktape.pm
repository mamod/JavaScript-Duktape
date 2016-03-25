package JavaScript::Duktape;
use strict;
use warnings;
use Carp;
use Data::Dumper;
use Scalar::Util 'looks_like_number';
our $VERSION = '1.0.0_5';

my $GlobalRef = {};
my $THIS;
my $DUKTAPE;
my $isNew  = bless [], "NEW";
my $HEAP   = bless [], "HEAP";
my $DUK    = bless [],  "DUK";
my $NOARGS = bless [], "NOARGS";

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
    _
    this
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
    my $duk = $self->{duk} = JavaScript::Duktape::Vm->new();
    $self->{pid} = $$;

    # Initialize global stash 'PerlGlobalStash'
    # this will be used to store some perl refs
    $duk->push_global_stash();
    $duk->push_object();
    $duk->put_prop_string(-2, "PerlGlobalStash");
    $duk->pop();

    $THIS = bless { sub => 1, duk => $duk, heapptr => 0 }, "JavaScript::Duktape::Object";

    ##global methods

    ##finalizer method
    $self->{finalizer} = sub {
        my $ref = $duk->get_string(0);
        delete $GlobalRef->{$ref};
        return 1;
    };

    $duk->perl_push_function($self->{finalizer}, 1);
    $duk->put_global_string('perlFinalizer');

    ##calling cached functions
    $self->{call} = sub {
        my $heapptr = $duk->get_heapptr(0);
        my $name    = $duk->get_string(1);
        my $top     = $duk->get_top();

        shift if ref $_[0] eq 'JavaScript::Duktape::Cache';

        $THIS->{heapptr} = $heapptr;
        $THIS->{duk}     = $duk;

        my @args;
        if (!@_){
            for (my $i = 2; $i < $top; $i++){
                push @args, $duk->to_perl($i);
            }
        } else {
            @args = @_;
        }

        my $ret = $GlobalRef->{$name}->{sub}->(@args);
        $duk->push_perl($ret);
        return 1;
    };

    $duk->push_perl_function($self->{call}, -1, 1);
    $duk->put_global_string('perlCall');

    return $self;
}

sub null  { $JavaScript::Duktape::NULL::null; }
sub true  { $JavaScript::Duktape::Bool::true; }
sub false { $JavaScript::Duktape::Bool::false }
sub JavaScript::Duktape::_   { $NOARGS }
sub this { $THIS }

sub set {
    my $self = shift;
    my $name = shift;
    my $val = shift;
    my $duk = $self->vm;

    if ($name =~ /\./){

        my @props = split /\./, $name;
        my $last = pop @props;
        my $others = join '.', @props;

        if ($duk->peval_string($others) != 0){
            croak $others . " is not a javascript object ";
        }

        my $type = $duk->get_type(-1);
        if ($type != DUK_TYPE_OBJECT){
            croak $others . " isn't an object";
        }

        $duk->push_string($last);
        $duk->push_perl($val);
        $duk->put_prop(-3);
        $duk->pop();
        return 1;
    }

    $duk->push_perl($val);
    $duk->put_global_string($name);
    return 1;
}

sub get {
    my $self = shift;
    my $name = shift;
    my $duk = $self->vm;
    $duk->push_string($name);
    if ($duk->peval() != 0) {
        croak $duk->safe_to_string(-1);
    }
    my $ret = $duk->to_perl(-1);
    $duk->pop();
    return $ret;
}

sub get_object {
    my $self = shift;
    my $name = shift;
    my $duk = $self->vm;
    $duk->push_string($name);
    if ($duk->peval() != 0) {
        croak $duk->safe_to_string(-1);
    }
    my $ret = $duk->to_perl_object(-1);
    $duk->pop();
    return $ret;
}

##FIXME : should pop here?
sub eval {
    my $self = shift;
    my $string = shift;
    my $duk = $self->duk;

    my $err = $duk->peval_string($string);

    if ($err){
        my $error_string = $duk->safe_to_string(-1);
        croak $error_string;
    }

    return $duk->to_perl(-1);
}

sub vm  { shift->{duk}; }
sub duk { shift->{duk}; }

sub DESTROY {
    my $self = shift;
    my $duk = $self->duk;
    if ($self->{pid} == $$){
        $duk->destroy_heap();
    }
}

package JavaScript::Duktape::Vm;
use strict;
use warnings;
no warnings 'redefine';
use Scalar::Util 'looks_like_number';
use Data::Dumper;
use Config;
use JavaScript::Duktape::C::libPath;
use Carp;

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
    # myextlib => $Duklib,
    # LIBS => '-L'. JavaScript::Duktape::C::libPath::getPath('../C') . ' -lduktape';

use Inline C => JavaScript::Duktape::C::libPath::getPath('duktape_wrap.c');
use Carp;

sub push_perl {
    my $self = shift;
    my $val = shift;

    if (my $ref = ref $val){
        if ($ref eq 'JavaScript::Duktape::NULL'){
            $self->push_null();
        }

        elsif ($ref eq 'JavaScript::Duktape::Bool'){
            if ($val){
                $self->push_true();
            } else {
                $self->push_false();
            }
        }

        elsif ($ref eq 'ARRAY'){
            my $arr_idx = $self->push_array();
            my $len = scalar @{$val};
            for (my $idx = 0; $idx < $len; $idx++) {
                $self->push_perl($val->[$idx]);
                $self->put_prop_index($arr_idx, $idx);
            }
        }

        elsif ($ref eq 'HASH'){
            $self->push_object();
            while (my ($k, $v) = each %{$val}) {
                $self->push_string($k);
                $self->push_perl($v);
                $self->put_prop(-3);
            }
        }

        elsif ($ref eq 'CODE') {
            $self->push_function($val);
        }

        elsif ($ref eq 'JavaScript::Duktape::Object'){
            $self->push_heapptr($val->{heapptr});
        }

        elsif ($ref eq 'JavaScript::Duktape::Function'){
            $self->push_heapptr($val->($HEAP));
        }

        elsif ($ref eq 'JavaScript::Duktape::Pointer'){
            $self->push_pointer($$val);
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

sub to_perl_object {
    my $self = shift;
    my $index = shift;
    my $heapptr = $self->require_heapptr($index);

    return JavaScript::Duktape::Util::jsObject({
        duk => $self,
        heapptr => $heapptr
    });
}

sub to_perl {
    my $self = shift;
    my $index = shift;
    my $prev = shift;
    my $ret;

    my $type = $self->get_type($index);

    if ($type == JavaScript::Duktape::DUK_TYPE_UNDEFINED){
        return;
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_STRING){
        $ret = $self->get_utf8_string($index);
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_NUMBER){
        $ret = $self->get_number($index);
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_OBJECT ||
            $type == JavaScript::Duktape::DUK_TYPE_BUFFER){

        if ($self->is_function($index)){
            my $ptr = $self->get_heapptr($index);
            return sub {
                $self->push_heapptr($ptr);
                $self->push_this();
                my $len = 0+@_;
                for (my $i = 0; $i < $len; $i++){
                    $self->push_perl($_[$i]);
                }
                if ($self->pcall_method($len) == 1) {
                    croak $self->safe_to_string(-1);
                }
                my $ret = $self->to_perl(-1);
                $self->pop();
                return $ret;
            };
        }

        my $isArray = $self->is_array($index);
        $ret = $isArray ? [] : {};
        $self->enum($index, JavaScript::Duktape::DUK_ENUM_OWN_PROPERTIES_ONLY);

        while ($self->next(-1, 1)) {
            my ($key, $val);

            #I'm not sure why I need to
            #substract 4 from stack top!!
            my $top = $self->get_top() - 4;

            $key = $self->to_perl(-2);
            my $found = 0;
            while ($top--){
                last if $top == -1;
                if ($self->strict_equals(-1, $top)){
                    $found = 1;
                    $val = defined $prev ? $prev : $ret;
                    last;
                }
            }

            ##no circular object found
            $val = $self->to_perl(-1, $ret) if !$found;

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
        $ret = JavaScript::Duktape::NULL::null();
    }

    elsif ($type == JavaScript::Duktape::DUK_TYPE_POINTER){
        my $p = $self->get_pointer($index);
        $ret = bless \$p, 'JavaScript::Duktape::Pointer';
    }

    return $ret;
}

##############################################
# push functions
##############################################
sub push_function {
    my $self  = shift;
    my $sub   = shift;
    my $nargs = shift || -1;

    $self->push_c_function(sub {
        my @args;
        my $top = $self->get_top();
        for (my $i = 0; $i < $top; $i++){
            push @args, $self->to_perl($i);
        }

        $self->push_this();
        my $heap = $self->get_heapptr(-1);
        $self->pop();

        if (!$heap){
            $self->push_global_object();
            $heap = $self->get_heapptr(-1);
            $self->pop();
        }

        $THIS->{heapptr} = $heap;
        $THIS->{duk}     = $self;

        my $ret = $sub->(@args);
        $self->push_perl($ret);
        return 1;
    }, $nargs);
}

sub push_c_function {
    my $self  = shift;
    my $sub   = shift;
    my $nargs = shift || -1;
    my $nofinalizer = shift;

    $GlobalRef->{"$sub"} = sub {
        my @args = @_;
        my $top = $self->get_top();
        my $ret = 1;
        my $died;
        $self->perl_duk_safe_call(sub {
            eval { $ret = $sub->(@args) };
            my $error = $@;
            if ($error){
                if ($error =~ /^Duk::Error/){
                    croak $@;
                } else {
                    ## don't throw error inside perl sub safe call
                    ## just tell our sub that there is an error and
                    ## let it exit this calling stack then throw
                    ## Fixes issue #6
                    $died = $error;
                    $self->eval_string('(function (e){ return new Error(e) })');
                    $self->push_string($error);
                    $self->call(1);
                }
            }
            return $ret;
        }, $top, 1);

        if ($died){
            $self->push_error_object(100, $died);
            croak $died;
        }

        return $ret;
    };

    $self->perl_push_function($GlobalRef->{"$sub"}, $nargs);

    return if $nofinalizer;
    $self->eval_string("(function(){perlFinalizer('$sub')})");
    $self->set_finalizer(-2);
}

sub cache {
    my $self = shift;
    my $sub = shift;

    my @caller = caller;
    my $code_cache_name = $caller[0] . $caller[2];

    if (!$GlobalRef->{$code_cache_name}){
        $GlobalRef->{$code_cache_name} = bless {
            duk => $self,
            sub => $sub,
            name => $code_cache_name
        }, "JavaScript::Duktape::Cache";
    }
    $GlobalRef->{$code_cache_name}->{args} = \@_;
    return $GlobalRef->{$code_cache_name};
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
            $self->eval_string("var t = new Error('$error'); t;");
        }
        return 1;
    }
    return 0;
}

##############################################
# custom functions
##############################################
*get_utf8_string = \&perl_duk_get_utf8_string;
*push_perl_function = \&push_c_function;
*push_light_function = \&perl_push_function;

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

package JavaScript::Duktape::NULL; {
    use warnings;
    use strict;
    our ($null);
    use overload
        '""' => sub { ${$_[0]} },
        'bool' => sub { ${$_[0]} ? 1 : 0 },
        fallback => 1;

    BEGIN {
        my $n = '';
        $null  = bless \$n, 'JavaScript::Duktape::NULL';
    }

    sub null  { $null }
}

package JavaScript::Duktape::Object; {
    use warnings;
    use strict;
    use Carp;
    use Data::Dumper;
    my $CONSTRUCTORS = {};
    use Scalar::Util 'weaken';
    use overload
        '""' => sub {
            my $self = shift;
            $self->inspect();
        },
        fallback => 1;

    sub inspect {
        my $self = shift;
        my $heapptr = $self->{heapptr};
        my $duk = $self->{duk};
        $duk->push_heapptr($heapptr);
        my $ret = $duk->to_perl(-1);
        $duk->pop();
        return $ret;
    }

    our $AUTOLOAD;
    sub AUTOLOAD {
        my $self = shift;
        my $heapptr = $self->{heapptr};
        my $duk = $self->{duk};
        my ($method) = ($AUTOLOAD =~ /([^:']+$)/);
        return if $method eq 'DESTROY';
        return JavaScript::Duktape::Util::autoload($self, $method, $duk, $heapptr, @_);
    }

    DESTROY {
        my $self = shift;
        my $duk = $self->{duk};
        my $heapptr = $self->{heapptr};

        #don't free if this coming from sub
        return if $self->{sub};

        ##TODO: Move these to C
        $duk->push_global_stash();
        $duk->get_prop_string(-1, "PerlGlobalStash");
        $duk->push_number($heapptr);
        $duk->del_prop(-2);
        $duk->pop_2();
    }
}

package JavaScript::Duktape::Function; {
    use strict;
    use warnings;
    use Data::Dumper;

    sub new {
        my $self = shift;
        $self->($isNew, @_);
    }

    our $AUTOLOAD;
    sub AUTOLOAD {
        my $self    = shift;
        my $heapptr = $self->($HEAP);
        my $duk     = $self->($DUK);

        my ($method) = ($AUTOLOAD =~ /([^:']+$)/);
        return if $method eq 'DESTROY';
        return JavaScript::Duktape::Util::autoload($self, $method, $duk, $heapptr, @_);
    }

    sub DESTROY {}
};

package JavaScript::Duktape::Util; {
    use strict;
    use warnings;
    use Data::Dumper;
    use Carp;

    sub autoload {
        my $self    = shift;
        my $method  = shift;
        my $duk     = shift;
        my $heapptr = shift;

        $duk->push_heapptr($heapptr);
        if ($method eq 'new'){
            my $len = @_ + 0;
            foreach my $val (@_){
                $duk->push_perl($val);
            }
            if ($duk->pnew($len) != 0){
                croak $duk->safe_to_string(-1);
            }
            my $val = $duk->to_perl_object(-1);
            $duk->pop();
            return $val;
        }

        my $val = undef;
        $duk->get_prop_string(-1, $method);

        my $type = $duk->get_type(-1);
        if ($type == JavaScript::Duktape::DUK_TYPE_OBJECT ||
             $type == JavaScript::Duktape::DUK_TYPE_BUFFER){

            if ($duk->is_function(-1)){
                my $function_heap = $duk->get_heapptr(-1);

                if (@_){
                    #called with special no arg _
                    shift if (ref $_[0] eq 'NOARGS');
                    $val = jsFunction($duk, $function_heap, $heapptr, 'call', @_);
                } else {
                    $val = jsFunction($duk, $function_heap, $heapptr);
                }
            } else {
                $val = $duk->to_perl_object(-1);
            }
        } else {
            $val = $duk->to_perl(-1);
        }
        $duk->pop_2();
        return $val;
    }

    sub jsFunction {
        my $duk = shift;
        my $heapptr = shift;
        my $constructor = shift || $heapptr;
        my $doCall = shift;
        my $sub = sub {
            # check first value, if it a ref of NEW
            # then this is a constructor call, other wise
            # it's just a normal call
            my $isNew;
            my $ref = ref $_[0];
            if ($ref eq "NEW"){
                shift; $isNew = 1;
            } elsif ($ref eq "HEAP"){
                return $heapptr;
            } elsif ($ref eq "DUK"){
                return $duk;
            }

            my $len = @_ + 0;
            $duk->push_heapptr($heapptr);
            $duk->push_heapptr($constructor) if !$isNew;
            foreach my $val (@_){
                if (ref $val eq 'CODE'){
                    $duk->push_function($val);
                } elsif (ref $val eq 'JavaScript::Duktape::Cache') {
                    my $name = $val->{name};
                    $duk->eval_string(qq~
                        (function (a, b, c, d){
                            var name = "$name";
                            return perlCall(this, name, a, b, c, d);
                        });
                    ~);
                } else {
                    $duk->push_perl($val);
                }
            }

            if ($isNew){
                if ($duk->pnew($len) != 0){
                    croak $duk->safe_to_string(-1);
                }
            } else {
                if ($duk->pcall_method($len) != 0){
                    croak $duk->safe_to_string(-1);
                }
            }

            my $ret;
            ##getting function call values
            my $type = $duk->get_type(-1);
            if ($type == JavaScript::Duktape::DUK_TYPE_OBJECT ||
                 $type == JavaScript::Duktape::DUK_TYPE_BUFFER){
                $ret = $duk->to_perl_object(-1);
            } else {
                $ret = $duk->to_perl(-1);
            }
            $duk->pop();
            return $ret;
        };

        return $sub->(@_) if $doCall;
        return bless $sub, "JavaScript::Duktape::Function";
    }

    sub jsObject {
        my $options = shift;

        my $duk         = $options->{duk};
        my $heapptr     = $options->{heapptr};
        my $constructor = $options->{constructor} || $heapptr;
        ##TODO: Move these to C
        # to prevent duktape garbage collecting this object
        # while it's still used in perl land we need to store
        # it in perl global stash, this will be freed once the
        # destroy method called of "JavaScript::Duktape::Object"
        $duk->push_global_stash();
        $duk->get_prop_string(-1, "PerlGlobalStash");
        $duk->push_number($heapptr);
        $duk->push_heapptr($heapptr);
        $duk->put_prop(-3); #PerlGlobalStash[heapptr] = object
        $duk->pop_2();

        my $type = $duk->get_type(-1);

        ##if this is a function return sub immediately
        ##other wise return blessed package
        if ($duk->is_function(-1)){
            return JavaScript::Duktape::Util::jsFunction($duk, $heapptr, $constructor);
        }

        return bless {
            duk => $duk,
            heapptr => $heapptr
        }, "JavaScript::Duktape::Object";
    }
}

1;

__END__
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
        my $duk = shift;
        print $_[0], "\n";
    });

    $js->eval(qq~
        (function(){
            for (var i = 0; i < 100; i++){
                write(i);
            }
        })();
    ~);

=head1 DESCRIPTION

JavaScript::Duktape implements almost all duktape javascript engine api, the c code is just
a thin layer that maps duktape api to perl, and all other functions implemented in perl
it self, so maintaing and contributing to the base code should be easy.

=head1 methods

=over 4

=item set('name', data);

Creates properity 'name' and sets it's value to the given perl data

=item get('name');

Gets properity 'name' value from javascript and return it as perl data

=item eval('javascript');

Evaluates javascript string and return the results or croak if error

=back

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

=head1 VM methods

As a general rule all duktape api supported, but I haven't had the chance to test them all,
so please report any missing or failure api call and I'll try to fix

For the list of duktape engine API please see L<http://duktape.org/api.html>, and here is how
you can translate duktape api to perl

    my $js = JavaScript::Duktape->new();
    my $duk = $js->duk;

    # -- C example
    # duk_push_c_function(func, 2);
    # duk_push_int(ctx, 2);
    # duk_push_int(ctx, 3);
    # duk_call(ctx, 2);  /* [ ... func 2 3 ] -> [ 5 ] */
    # printf("2+3=%ld\n", (long) duk_get_int(ctx, -1));
    # duk_pop(ctx);

    #and here is how we can implement it in JavaScript::Duktape

    $duk->push_c_function(sub {
        my $duk = shift;
        my $num1 = $duk->get_int(0);
        my $num2 = $duk->get_int(1);

        my $total = $num1+$num2;
        $duk->push_number($total);
        return 1;
    }, 2);

    $duk->push_int(2);
    $duk->push_int(3);
    $duk->call(2);  # [ ... func 2 3 ] -> [ 5 ]
    printf("2+3=%ld\n", $duk->get_int(-1));
    $duk->pop();

As you can see all you need to do is replacing C<duk_> with C<$duk->> and remove C<ctx> from the function call,
this may sounds crazy but api tests have been generated by copying duktape tests and using search and replace tool :)

Besides duktape api, C<JavaScript::Duktape::Vm> implements the following methods

=over 4

=item push_function ( code_ref, num_of_args );

an alias to push_c_function

=item push_perl( ... );

Push given perl data into the duktape stack.

=item to_perl(index);

Get the value at index and return it as perl data

=item reset_top

resets duktape stack top

=back

=head1 AUTHOR

Mamod Mehyar C<< <mamod.mehyar@gmail.com> >>

=head1 LICENSE

This library is free software; you can redistribute it and/or modify it under the same terms as Perl itself.
