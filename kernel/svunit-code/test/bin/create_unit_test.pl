#!/usr/bin/perl

################################################################
#
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#  http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#
################################################################

use File::Basename;


##########################################################################
# Variables
##########################################################################
my $num_tests   = 0;
my $total_tests = 0;
my $includes_already_printed = 0;


##########################################################################
# PrintHelp(): Prints the script usage.
##########################################################################
sub PrintHelp() {
  print "\n";
  print "Usage:  create_unit_test.pl [ -help | -out <file> | -i | -overwrite | uut.sv ]\n\n";
  print "Where -help                : prints this help screen\n";
  print "      -out <file>          : specifies a new default output file\n";
  print "      -overwrite           : overwrites the output file if it already exists\n";
  #print "      -class_name <name>   : generate a unit test template for a class <name>\n";
  #print "      -module_name <name>  : generate a unit test template for a module <name>\n";
  #print "      -if_name <name>      : generate a unit test template for an interface <name>\n";
  print "      uut.sv               : the file with the unit under test\n";
  print "\n";
}


##########################################################################
# CheckArgs(): Checks the arguments of the program.
##########################################################################


sub CheckArgs() {
  $numargs = $#ARGV+1;

  for $i (0..$numargs-1) {
    if ( $skip == 1 ) {
      $skip = 0;
    }
    else {
      if ( @ARGV[$i] =~ /-help/ ) {
        PrintHelp();
      }
      elsif ( @ARGV[$i] =~ /-out/ ) {
        $i++;
        $skip = 1;
        $output_file = $ARGV[$i];
      }
      elsif ( @ARGV[$i] =~ /-class_name/ ) {
        $i++;
        $skip = 1;
        $class_name = $ARGV[$i];
      }
      elsif ( @ARGV[$i] =~ /-module_name/ ) {
        $i++;
        $skip = 1;
        $module_name = $ARGV[$i];
      }
      elsif ( @ARGV[$i] =~ /-if_name/ ) {
        $i++;
        $skip = 1;
        $if_name = $ARGV[$i];
      }
      elsif ( @ARGV[$i] =~ /-overwrite/ ) {
        $overwrite = 1;
      }
      else {
        if ( -r "@ARGV[$i]" ) {
          $testname = @ARGV[$i];
        }
      }
    }
  }
}

  
##########################################################################
# ValidArgs(): This checks to see if the arguments provided make sense.
##########################################################################
sub ValidArgs() {
  if ( not defined($testname) and not defined($class_name) and not defined($module_name) and not defined($if_name)) {
    print "\nERROR:  The testfile was either not specified, does not exist or is not readable\n";
    PrintHelp();
    return 1;
  }
  if (defined ($class_name)) {
    $output_file = $class_name;
    $output_file .= "_unit_test.sv";
  }
  elsif (defined ($module_name)) {
    $output_file = $module_name;
    $output_file .= "_unit_test.sv";
  }
  elsif (defined ($if_name)) {
    $output_file = $if_name;
    $output_file .= "_unit_test.sv";
  }
  elsif ($output_file eq "") {
    ($name, $path, $suffix) = fileparse($testname, qr/\.[^.]*/);
    $output_file = "$name";
    $output_file .= "_unit_test.sv";
  }
  else {
    if ($output_file !~ m/_unit_test\.sv$/) {
      print "\nERROR:  The output_file '$output_file' must end in '_unit_test.sv'\n";
      return 1;
    }
  }
  return 0;
}


##########################################################################
# OpenFiles(): This opens the input and output files
##########################################################################
sub OpenFiles() {
  if (defined ($testname) ) {
    open (INFILE,  "$testname")     or die "Cannot Open file $testname\n";
  }
  if ( -r $output_file and $overwrite != 1 ) {
    print "\nERROR: The output file '$output_file' already exists, to overwrite, use the -overwrite argument\n\n";
    exit 1;
  }
  else {
    open (OUTFILE, ">$output_file") or die "Cannot Open file $output_file\n";
  }
}


##########################################################################
# CloseFiles(): This closes the input and output files
##########################################################################
sub CloseFiles() {
  if (defined $testname) {
    close (INFILE)  or die "Cannot Close file $testname\n";
  }
  close (OUTFILE) or die "Cannot Close file $output_file\n";
}


##########################################################################
# Main(): writes the rest of the unit test file
##########################################################################
sub Main() {
  $incomments = 0;
  while ($line = <INFILE> ) {
    # if a /* */ comment is still open, look for the end
    if ($incomments) {
      if ($line =~ m|.*\*/|) {
        $line =~ s|.*\*/||;
        $incomments = 0;
      }
    }

    if (!$incomments) {
      # filter full /* */ comments
      $line =~ s|/\*.*?\*/||g;
   
      # filter // comments
      $line =~ s|//.*||;

      # filter /* comments that go past the end of a line
      if ( $line =~ /\/\*/) {
        $line =~ s/\/\*.*//;
        $incomments = 1;
      }

      if ( $processing_uut == 0 ) {
        if ( $line =~ /^\s*class\s/ ) {
          $line =~ s/\<virtual\>//g;
          $line =~ s/^\s*class/class/g;
          $line =~ s/\s+/ /g;
          $line =~ s/\W/:/g;
          @items = split(/:/, $line);
          $uut = $items[1];
          $processing_class = 1;
          $processing_uut = 1;
        }
        elsif ( $line =~ /^\s*module\s/ ) {
          $line =~ s/^\s*module/module/g;
          $line =~ s/\s+/:/g;
          $line =~ s/\W/:/g;
          @items = split(/:/, $line);
          $uut = $items[1];
          $processing_module = 1;
          $processing_uut = 1;
        }
        elsif ( $line =~ /^\s*interface\s/ ) {
          $line =~ s/^\s*interface/interface/g;
          $line =~ s/\sstatic\s/ /;
          $line =~ s/\sautomatic\s/ /;
          $line =~ s/\s+/:/g;
          $line =~ s/\W/:/g;
          @items = split(/:/, $line);
          $uut = $items[1];
          $processing_if = 1;
          $processing_uut = 1;
        }
      }
      else {
        if ( $processing_class && $line =~ /^\s*endclass/ ) {
          $testfilename = basename ($testname);
          CreateUnitTest();
          $total_tests = $total_tests + $num_tests;
          $num_tests = 0;
          $processing_class = 0;
          $processing_uut = 0;
        }

        elsif ( $processing_module && $line =~ /^\s*endmodule/ ) {
          $testfilename = basename ($testname);
          CreateUnitTest();
          $total_tests = $total_tests + $num_tests;
          $num_tests = 0;
          $processing_module = 0;
          $processing_uut = 0;
        }

        elsif ( $processing_if && $line =~ /^\s*endinterface/ ) {
          $testfilename = basename ($testname);
          CreateUnitTest();
          $total_tests = $total_tests + $num_tests;
          $num_tests = 0;
          $processing_if = 0;
          $processing_uut = 0;
        }
      }
    }
  }

  if (defined ($class_name)) {
    $processing_class = 1;
    $uut = $class_name;
    $testfilename = "$class_name.sv";
    CreateUnitTest();
  }
  elsif (defined ($module_name)) {
    $processing_module = 1;
    $uut = $module_name;
    $testfilename = "$module_name.sv";
    CreateUnitTest();
  }
  elsif (defined ($if_name)) {
    $processing_if = 1;
    $uut = $if_name;
    $testfilename = "$if_name.sv";
    CreateUnitTest();
  }
}


##########################################################################
# CreateClassUnitTest(): This creates the output for the unit test class.  It's
#                   called for each class within the file
##########################################################################
sub CreateUnitTest() {
  $in_list = 0;

  if (!$includes_already_printed) {
    print OUTFILE "`include \"svunit_defines.svh\"\n";
    print OUTFILE qq|`include \"$testfilename\"\n|;
    $includes_already_printed = 1;
  }
  print OUTFILE "\n";
  print OUTFILE "import svunit_pkg::\*;\n\n";
  print OUTFILE "\n";
  print OUTFILE "module $uut\_unit_test;\n\n";
  print OUTFILE "  string name = \"$uut\_ut\";\n";
  print OUTFILE "  svunit_testcase svunit_ut;\n";
  print OUTFILE "\n";
  print OUTFILE "\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  // This is the UUT that we're \n";
  print OUTFILE "  // running the Unit Tests on\n";
  print OUTFILE "  //===================================\n";
  if ($processing_class) {
    print OUTFILE "  $uut my_$uut;\n\n\n";
  } else {
    print OUTFILE "  $uut my_$uut();\n\n\n";
  }
  print OUTFILE "  //===================================\n";
  print OUTFILE "  // Build\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  function void build();\n";
  print OUTFILE "    svunit_ut = new(name);\n";
  if ($processing_class) {
    print OUTFILE "\n";
    print OUTFILE "    my_$uut = new(\/\* New arguments if needed \*\/);\n";
  }
  print OUTFILE "  endfunction\n\n\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  // Setup for running the Unit Tests\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  task setup();\n";
  print OUTFILE "    svunit_ut.setup();\n";
  print OUTFILE "    \/\* Place Setup Code Here \*\/\n  endtask\n\n\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  // Here we deconstruct anything we \n";
  print OUTFILE "  // need after running the Unit Tests\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  task teardown();\n";
  print OUTFILE "    svunit_ut.teardown();\n";
  print OUTFILE "    \/\* Place Teardown Code Here \*\/\n";
  print OUTFILE "  endtask\n\n\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  // All tests are defined between the\n";
  print OUTFILE "  // SVUNIT_TESTS_BEGIN/END macros\n";
  print OUTFILE "  //\n";
  print OUTFILE "  // Each individual test must be\n";
  print OUTFILE "  // defined between `SVTEST(_NAME_)\n";
  print OUTFILE "  // `SVTEST_END\n";
  print OUTFILE "  //\n";
  print OUTFILE "  // i.e.\n";
  print OUTFILE "  //   `SVTEST(mytest)\n";
  print OUTFILE "  //     <test code>\n";
  print OUTFILE "  //   `SVTEST_END\n";
  print OUTFILE "  //===================================\n";
  print OUTFILE "  `SVUNIT_TESTS_BEGIN\n\n\n\n";
  print OUTFILE "  `SVUNIT_TESTS_END\n\n";

  print OUTFILE "endmodule\n";
}


##########################################################################
# This is the main run flow of the script
##########################################################################
CheckArgs();
if ( ValidArgs() == 0) {
  OpenFiles();
  Main();
  CloseFiles(); 
}
