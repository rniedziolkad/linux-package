#!/usr/bin/perl -w

# GIMP - The GNU Image Manipulation Program
# Copyright (C) 1999-2003 Manish Singh <yosh@gimp.org>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUTFILE ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

BEGIN {
    $srcdir   = $ENV{srcdir}   || '.';
    $destdir  = $ENV{destdir}   || '.';
    $builddir = $ENV{builddir} || '.';
}

use lib $srcdir;

use Text::Wrap qw(wrap $columns);
$columns = 77;

#BEGIN { require 'util.pl' }
require 'util.pl';

*write_file = \&Gimp::CodeGen::util::write_file;
*FILE_EXT   = \$Gimp::CodeGen::util::FILE_EXT;

my $header = <<'HEADER';
:# GIMP - The GNU Image Manipulation Program
:# Copyright (C) 1999-2003 Manish Singh <yosh@gimp.org>
:
:# This program is free software: you can redistribute it and/or modify
:# it under the terms of the GNU General Public License as published by
:# the Free Software Foundation; either version 3 of the License, or
:# (at your option) any later version.
:
:# This program is distributed in the hope that it will be useful,
:# but WITHOUTFILE ANY WARRANTY; without even the implied warranty of
:# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
:# GNU General Public License for more details.
:
:# You should have received a copy of the GNU General Public License
:# along with this program.  If not, see <https://www.gnu.org/licenses/>.
:
:# autogenerated by enumgen.pl
:
:package Gimp::CodeGen::enums;
:
:%enums = (
HEADER

my $external;
open my $EXTERNAL, "enums-external.pl";
{
    local $/;
    $external = <$EXTERNAL>;
}
close $EXTERNAL;

my $footer = <<'FOOTER';
:);
:
:foreach $e (values %enums) {
:    $e->{info} = "";
:    foreach (@{$e->{symbols}}) {
:	$e->{info} .= "$_ ($e->{mapping}->{$_}), "
:    }
:    $e->{info} =~ s/, $//;
:}
:
:1;
FOOTER

my ($enumname, $contig, $symbols, @mapping, $before);

# Most of this enum parsing stuff was swiped from makeenums.pl in GTK+
sub parse_options {
    my $opts = shift;
    my @opts;

    for $opt (split /\s*,\s*/, $opts) {
	$opt =~ s/^\s*//;
	$opt =~ s/\s*$//;
	my ($key,$val) = $opt =~ /([-\w]+)(?:=(.+))?/;
	defined $val or $val = 1;
	push @opts, $key, $val;
    }
    @opts;
}

sub parse_entries {
    my $file = shift;
    my $file_name = shift;
    my $looking_for_name = 0;

    while (<$file>) {
	# Read lines until we have no open comments
	while (m@/\*([^*]|\*(?!/))*$@) {
	    my $new;
	    defined ($new = <$file>) || die "Unmatched comment in $ARGV";
	    $_ .= $new;
	}
	# strip comments w/o options
	s@/\*(?!<)
	    ([^*]+|\*(?!/))*
	   \*/@@gx;
        
	s@\n@ @;
        
	next if m@^\s*$@;

	if ($looking_for_name) {
	    if (/^\s*(\w+)/) {
		$enumname = $1;
		return 1;
	    }
	}

	# Handle include files
	if (/^\#include\s*<([^>]*)>/ ) {
	    my $file= "../$1";
	    open NEWFILE, $file or die "Cannot open include file $file: $!\n";

	    if (&parse_entries (\*NEWFILE, $NEWFILE)) {
		return 1;
	    } else {
		next;
	    }
	}

	if (/^\s*\}\s*(\w+)/) {
	    $enumname = $1;
	    return 1;
	}

	if (/^\s*\}/) {
	    $looking_for_name = 1;
	    next;
	}

	if (m@^\s*
	      (\w+)\s*                   # name
	      (?:=(                      # value
		   (?:[^,/]|/(?!\*))*
		  ))?,?\s*
	      (?:/\*<                    # options 
		(([^*]|\*(?!/))*)
	       >\s*\*/)?,?
	      \s*$
             @x) {
            my ($name, $value, $options) = ($1, $2, $3);

	    if (defined $options) {
		my %options = parse_options($options);
		next if defined $options{"pdb-skip"};
	    }

	    $symbols .= $name . ' ';

	    # Figure out a default value (not really foolproof)
	    $value = $before + 1 if !defined $value;
	    $value =~ s/\s*$//s;
	    $value =~ s/^\s*//s;

	    push @mapping, $name, $value;

	    my $test = $before + 1;

	    # Warnings in our eval should be fatal so they set $@
	    local $SIG{__WARN__} = sub { die $_[0] };

	    # Try to get a numeric value
	    eval "\$test = $value * 1;";

	    # Assume noncontiguous if it's not a number
	    $contig = 0 if $contig && ($@ || $test - 1 != $before);

	    $before = $test;
	} elsif (m@^\s*\#@) {
	    # ignore preprocessor directives
        } else {
            print STDERR "$0: $file_name:$.: Failed to parse `$_'\n";
        }
    }
    return 0;
}

my $code = "";
while (<>) {
    if (eof) {
        close (ARGV);           # reset line numbering
    }

    # read lines until we have no open comments
    while (m@/\*([^*]|\*(?!/))*$@) {
	my $new;
	defined ($new = <>) || die "Unmatched comment in $ARGV";
	$_ .= $new;
    }
    # strip comments w/o options
    s@/\*(?!<)
       ([^*]+|\*(?!/))*
       \*/@@gx;

    if (m@^\s*typedef\s+enum\s*
	   ({)?\s*
	   (?:/\*<
	     (([^*]|\*(?!/))*)
	    >\s*\*/)?
         @x) {
        if (defined $2) {
            my %options = parse_options($2);
	    next if defined $options{"pdb-skip"};
	}	    
	# Didn't have trailing '{' look on next lines
	if (!defined $1) {
	    while (<>) {
		if (s/^\s*\{//) {
		    last;
		}
	    }
	}

	$symbols = ""; $contig = 1; $before = -1; @mapping = ();

	# Now parse the entries
	&parse_entries (\*ARGV, $ARGV);

	$symbols =~ s/\s*$//s;
	$symbols = wrap("\t\t\t  ", "\t\t\t  " , $symbols);
	$symbols =~ s/^\s*//s;

	my $mapping = ""; $pos = 1;
	foreach (@mapping) {
	    $mapping .= $pos++ % 2 ? "$_ => " : "'$_',\n\t\t       ";
	}
	$mapping =~ s/,\n\s*$//s;

	$ARGV =~ s@(?:(?:..|app)/)*@@;

	$code .= <<ENTRY;
:    $enumname =>
:	{ contig => $contig,
:	  header => '$ARGV',
:	  symbols => [ qw($symbols) ],
:	  mapping => { $mapping }
:	},
ENTRY
    }
}

$code =~ s/,\n$/\n/s;

foreach ($header, $code, $footer) { s/^://mg }

$outfile = "$builddir/pdb/enums.pl$FILE_EXT";
open OUTFILE, "> $outfile";
print OUTFILE $header, $external, $code, $footer;
close OUTFILE;
&write_file($outfile, "$destdir/pdb");
