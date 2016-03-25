##read api.html file dowloaded from duktape.org/api.html
##and produce fake api.h file which contains all duktape api calls
##should be called only once Duktape change api and api.html edited

use warnings;
use strict;
use jQuery;
use Data::Dumper;

open(my $file, '<', 'api.html') or die $!;
open(my $out, '>', 'api.h') or die $!;

my $html = do { local $/; <$file> };
close $file;

jQuery->new($html);
my $i = 0;
jQuery('.api-call .api-part:first .c-code')->each(sub {
    my $text =  'DUK_EXTERNAL_DECL ' . this->text();
    $text =~s/\n//g;

    if ($text =~ s/\{(\s?)$/;/){
        print $text;
    }

    $text .= "\n";
    ##disable debug methods
    if ($text !~ /duk_debug/){
        print $out $text;
    }
});
# print $html;
close $out;
