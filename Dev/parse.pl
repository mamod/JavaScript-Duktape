##parse api.h file and produce perl and c functions

use strict;
use warnings;
use Data::Dumper;

open(my $file, '<', 'api.h') or die $!;

my @map;
my @code;

while (<$file>){
    
    if ($_ =~ /^DUK_EXTERNAL_DECL/){
        $_ =~ s/^DUK_EXTERNAL_DECL(\s+?)//;
        $_ =~ s/;(\n)?$//;
        
        if ($_ !~ /duk_context \*ctx/g){
            next;
        }

        my $code = '//' . $_ . ";\n";
        my $isPointer = 0;

        my @split = split /\s+/, $_, 2;
        my $type = $split[0];
        my $function = $split[1];

        if ($function !~ /^duk_/){
            $function =~ s/(.*?)(duk_)/$2/;
            $type = $type . ' ' . $1;
        }

        if ($type =~ /\*$/){
            $isPointer = 1;
        }

        # print Dumper \@split;

        my $code_type = $isPointer ? $type : $type . " ";

        ##lstring
        my $lstring = 0;
        if ($function =~ s/duk_size_t \*/SV \*/){
            $function =~ s/SV \*out_size/SV \*out_len/;
            $lstring = 1;
            #die $function;
        }

        my $code_function = $function;
        $code_function =~ s/(.*?)\((.*?)\)/$1/;
        my $params = $2;

        ##parse params
        my @params = split ',', $params;
        # print Dumper \@params;
        my @code_params;
        foreach my $param (@params){

            my @single = split /\s+?/, $param;
            print Dumper \@single;
            my $v = $single[@single - 1];
            # print Dumper $v;
            $v =~ s/^\*//;
            if ($v eq '...'){ goto NEXT; }

            push @code_params, $v;
        }


        $code_function = $code_function . '(' . join(', ', @code_params) . ');';
        my $perl_function = $function;
        # $perl_function =~s/duk_context \*ctx/SV \*Obj/;
        $perl_function = 'aperl_' . $perl_function;

        
        $code .= $code_type . "$perl_function {\n";
        $code .= "\tduk_size_t sz;\n" if $lstring;

        # $code .= "\tDUK_PERL *js = (DUK_PERL*)SvIV(SvRV(Obj));\n";
        # $code .= "\tduk_context *ctx = js->ctx;\n";

        if ($type eq 'void'){
            $code .= "\t" . $code_function . "\n";
        } else {
            $code_function =~ s/out_len/&sz/ if $lstring;
            $code .= "\t" . $code_type . "ret = " . $code_function . "\n";
            $code .= "\tsv_setnv(out_len, sz);\n" if $lstring;
            $code .= "\treturn ret;\n";
        }

        $code .= "}";

        push @code, $code;

        ##perl map code
        $perl_function =~ s/(.*?)\(.*/$1/;
        my $fn = $perl_function;
        $fn =~ s/(.*?)duk_(.*)/$2/;
        my $map = "=item C<$fn>\n\n";
        $map .= "see L<http://duktape.org/api.html#duk_$fn>\n\n";
        $map .= "=cut\n";
        $map .= "*$fn = " . '\&' . "$perl_function;\n";
        push @map, $map;
        NEXT :{ 1 }
    }
}

#duktape_wrapper.h
my $joincode = join "\n\n", @code;
my $output = qq~
$joincode
~;

open (my $out, '>', 'duktape_wrapper.c') or die $!;
print $out $output;

# print Dumper join "\n", @map;
#FunctionsMap.pl
my $fjoin = join "\n", @map;
my $foutput = qq~
#This File auto generated
use strict;
use warnings;

=over

=cut

$fjoin

=back

=cut
1;
~;

open (my $fout, '>', 'FunctionsMap.pl') or die $!;
print $fout $foutput;

close $out;
close $fout;
