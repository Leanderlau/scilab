// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for archiveExtract function
// ============================================================================

x = archiveExtract("SCI/modules/fileio/tests/unit_tests/example.tar");
assert_checktrue(isfile("example1.txt"));
assert_checktrue(isfile("example2.txt"));
deletefile("example1.txt");
deletefile("example2.txt");

x = archiveExtract("SCI/modules/fileio/tests/unit_tests/example.tar",%f);
assert_checktrue(isfile("example1.txt"));
assert_checktrue(isfile("example2.txt"));
deletefile("example1.txt");
deletefile("example2.txt");

x = archiveExtract("SCI/modules/fileio/tests/unit_tests/example.tar",%t,"archiveExtract");
assert_checktrue(isfile("archiveExtract/example1.txt"));
assert_checktrue(isfile("archiveExtract/example2.txt"));
removedir("archiveExtract");

ierr = execstr("archiveExtract(SCI)","errcatch");
assert_checkequal(ierr,999);

ierr = execstr("archiveExtract(SCI,1)","errcatch");
assert_checkequal(ierr,999);
