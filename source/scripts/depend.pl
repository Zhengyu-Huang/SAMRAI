#! /usr/bin/perl
#########################################################################
##
## This file is part of the SAMRAI distribution.  For full copyright 
## information, see COPYRIGHT and COPYING.LESSER. 
##
## Copyright:     (c) 1997-2014 Lawrence Livermore National Security, LLC
## Description:   perl script to generate dependencies for SAMRAI files 
##
#########################################################################
##

# Output a Makefile.depend file for .C src files.  A dependency is
# created for each .C, .h, and .I recursively included file.
# In order to speed compilation the makefile flag 
# DEPENDS_ON_TEMPLATE_IMPLEMENTATION is used to control dependency
# on .C files.  The rules for determining dependency are: 
# 
# .h and .I files always included in the dependency
#
#  The src file is always included in the dependency.
#
#  .C files included directly by the src file are always included in 
#  the dependency.  
#
#  .C files that are recursively included are ONLY included in the 
#  dependency if DEPENDS_ON_TEMPLATE_IMPLEMENTATION is "yes".
#

use File::Basename;
$scriptname = basename($0);

$debug = 0;

$this_command = "$scriptname @ARGV";
$src_dir  = shift; $src_dir = '.' if "$src_dir" eq '';
$inc_dir  = shift; $inc_dir = '-' if "$inc_dir" eq '';
if ( @ARGV ) {
    @FILES   = sort @ARGV;
}
else {
    opendir( SRCDIR, $src_dir ) || die "Cannot open directory $src_dir";
    @FILES = sort grep( /.*\.[fCc]$/, (readdir SRCDIR) );
    closedir SRCDIR;
}
$DEPEND  = "Makefile.depend.tmp";

# By convention, directories should not end with '/'
# and relative paths should not start with './'.
# This is required to replace file searches with simpler string searches.
for ($src_dir, @FILES) { s|/+$||; s|^./+||o; }

if ( "$inc_dir" eq '-' ) {
    # Find the include directory by climbing back up the path,
    # past the source directory.
    $inc_dir = `pwd`; chop $inc_dir;
    if( $inc_dir =~ m|^.*/?source/?| ) {
	$inc_dir =~ s|^.*/?source/?|source/|o;	# This gets past dir source.
	$inc_dir =~ s|[^/]+|..|go;		# Change each generation to '..'.
	$inc_dir = "$inc_dir/source";    # Append include.
    } else {
	$inc_dir =~ s|^.*/?SAMRAI/?||o;	        # This gets past dir source.
	$inc_dir =~ s|[^/]+|..|go;		# Change each generation to '..'.
	$inc_dir = "$inc_dir/source";    # Append include.
    }
}

@INCPATH = ($src_dir, $inc_dir);
print "src_dir = $src_dir\n" if $debug;
print "inc_dir = $inc_dir\n" if $debug;
print "files = @FILES\n" if $debug;

$TABLEN  = 8;
$LINLEN  = 72;

#
# For each of the specified files, get dependency information and write to file
#

open(OUTFILE, ">$DEPEND") || die "Cannot open output file $DEPEND...";

$EMPTY="";

print OUTFILE <<__EOM__;
#########################################################################
##
## This file is part of the SAMRAI distribution.  For full copyright 
## information, see COPYRIGHT and COPYING.LESSER. 
##
## Copyright:     (c) 1997-2014 Lawrence Livermore National Security, LLC
## Description:   makefile dependencies
##
#########################################################################

## This file is automatically generated by $scriptname.


__EOM__

$FILENUMBER=0;

for $cfile (@FILES) {

    print "Checking file: $cfile\n" if $debug;

    undef %dset;	# The set of files that cfile depends on.
    @depfiles = ($cfile);	# List of files an object file compiled
    				# from $cfile would depend on.

    while (@depfiles) {
	$depfile = shift @depfiles;
	print "\tprocessing $depfile\n" if $debug;
	next if ( $depfile eq '' );
	if ( defined $dset{$depfile} ) {
	    print "\t$depfile is already in dependency set.\n" if $debug;
	    next;
	}
	# This file is not part of the dependency set.
	print "\t$depfile is being added the dependency set\n" if $debug;
	$dset{$depfile} = 1;	# Make current file a part of dependency set.
	# See what files $depfile depends on (and cache that info in
	# the variable filedeps).
	if ( ! defined $filedeps{$depfile} ) {
	    print "\tFinding filedeps for $depfile\n" if $debug;
	    $filedeps{$depfile} = [ &getMoreDeps( &getFullPath($depfile) ) ];
	}

	# See what other files $depfile depends on.
	for $maybedepfile ( @{$filedeps{$depfile}} ) {
	    $maybedepfile = &getFullPath($maybedepfile);
	    if ( $maybedepfile ne '' ) {
		print "\tWill also check file: $maybedepfile\n" if $debug;
		# Do not process the newly found include lines here.
		# Just add them to @depfiles to be processed by the
		# while loop.
		push( @depfiles, $maybedepfile )
		    if ! defined $dset{$maybedepfile}
	    }
	}
    }

    @deps = sort(keys %dset);
    for (@deps) { $_ = &fixName($_); }
    print "$cfile depends on @deps\n" if $debug;
    # Add SAMRAI_config.h because everything should depend on it,
    # even though it is ignored for the purpose of finding dependencies.
    # (It is ignored because it is generated at configure time.)
    unshift @deps, '$(OBJECT)/include/SAMRAI/SAMRAI_config.h';
    printDependencies( $FILENUMBER, $cfile, @deps );

    $FILENUMBER=$FILENUMBER + 1;
}


sub getMoreDeps {
    my $file = @_[0];
    @deps = ();
    if (open(DEPFILE, $file)) {
	while ( <DEPFILE> ) {
	    if ( s/^\s*\#\s*include\s*\"([^\"]+)\"\s*/\1/o
		 && /[^\s]/o
		 && ! /\.f$/o
		 ) {
		push( @deps, $_ )
		}
	}
	close DEPFILE;
	if ( $debug ) {
	    print "\t\t$file is recursively dependent on\n";
	    for ( @deps ) { print "\t\t\t$_\n"; }
	}
    }
    else {
	print "\t\tSkipping recursion on file: $file\n" if $debug;
    }
    return @deps;
}

sub getFullPath {
   my $FILE    = shift(@_);

   for (@INCPATH) {
       print "checking if $_/$FILE is in SAMRAI source\n" if $debug;
      if (-r "$_/$FILE") {
         return( $_ ne '.' ? "$_/$FILE" : $FILE );
      }
   }

   return("");
}


#
# Print out data dependencies in a pleasing manner
#

sub printDependencies {
   $FILENUMBER =  shift(@_);
   my $SRC_FILE = shift(@_);
   my @DEPS = @_;

   @C_SOURCE_DEPS=();
   @OTHER_DEPS=();

   $SRC_FILE = fixName($SRC_FILE);
   
   my $LIBLINE = $SRC_FILE;
   $LIBLINE =~ s/^(.*)\.[Cfc]/$1.o/o;
   print OUTFILE "FILE_$FILENUMBER=$LIBLINE\n";




   # Look for C files that are directly included by the 
   # src file.  Always include a dependency on these files.

   # Always always depend on the src file itself.
   @src_C_files=();
   push(@src_C_files, $SRC_FILE);
   @src_include_files= getMoreDeps( &getFullPath($SRC_FILE) );
   for(@src_include_files) {
       if(/.*\.C/) {
	   push(@src_C_files, "$_");
       }
   }
   my $src_include_patterns = join '|', @src_C_files;

   for(@DEPS) {
       if (/$SRC_FILE/) { 
	   # Always always depend on the src file itself.
	   push(@OTHER_DEPS, $_);
       } elsif (/$src_include_patterns/) { 
	   # Always depend on C files included directly by 
	   # the src file.
	   push(@OTHER_DEPS, $_);
       }
       elsif (/.*\.[C]/) {
	   push(@C_SOURCE_DEPS, $_);
       } else {
	   push(@OTHER_DEPS, $_);
       }
   }

   

   print OUTFILE "DEPENDS_$FILENUMBER:=\\\n";
   printTabbedFiles(@OTHER_DEPS);
   print OUTFILE "\n";
   print OUTFILE "ifeq (\${DEPENDS_ON_TEMPLATE_IMPLEMENTATION},yes)\n";
   print OUTFILE "DEPENDS_$FILENUMBER +=\\\n";
   printTabbedFiles(@C_SOURCE_DEPS);
   print OUTFILE "endif\n\n";
   
   $LIBLINE="\${FILE_$FILENUMBER:X.o=\${NDIM}.o}: \${DEPENDS_$FILENUMBER}";
   print OUTFILE "$LIBLINE\n\n";
}

sub SGSJUNK {
    my @JUNK = ("one", "two", "three");
    return  @JUNK;
}

sub printTabbedFiles {
   my @FILES = @_;
   print OUTFILE "\t";
   $CURLEN = $TABLEN;
   for (@FILES) {
      my $FILE = $_;
      if (length($FILE)+$CURLEN >= $LINLEN) {
         $NTAB = ($LINLEN-$CURLEN)/$TABLEN;
         for ($i = 0; $i < $NTAB; $i++) {
            print OUTFILE "\t";
         }
         print OUTFILE "\\\n";
         print OUTFILE "\t";
         $CURLEN = $TABLEN;
      }
      if ($CURLEN == $TABLEN) {
         print OUTFILE "$FILE";
         $CURLEN += length($FILE);
      } else {
         print OUTFILE " $FILE";
         $CURLEN += length($FILE)+1;
      }
   }
   print OUTFILE "\n";
}


#
# Convert the filename to a print name - remove include path or ./ prefix
#

sub fixName {
   $_ = shift(@_);
   print "Fixing name $_\n" if $debug;
   if ( m|(.*)/SAMRAI/(.*)$|o ) {
      return("\$(INCLUDE_SAM)/SAMRAI/$2");
   }
   if ( m|/([^/]*)$|o ) {
      return($1);
   } else {
      return($_);
   }
}
