async function updateContentOnceLoaded1()
{
    var data =`
==6310== Memcheck, a memory error detector<br />
==6310== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.<br />
==6310== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info<br />
==6310== Command: /home/runner/work/MeshToolBox/MeshToolBox/build/App/Test/AppUnitTests<br />
==6310== Parent PID: 6301<br />
==6310== <br />
==6310== <br />
==6310== <span class="valgrind_summary_title">HEAP SUMMARY:</span><br />
==6310== &nbsp; &nbsp; in use at exit: 0 bytes in 0 blocks<br />
==6310== &nbsp; total heap usage: 2,644 allocs, 2,644 frees, 497,390 bytes allocated<br />
==6310== <br />
==6310== All heap blocks were freed -- no leaks are possible<br />
==6310== <br />
==6310== For lists of detected and suppressed errors, rerun with: -s<br />
==6310== <span class="valgrind_summary_title">ERROR SUMMARY:</span> 0 errors from 0 contexts (suppressed: 0 from 0)<br />
`;
    var analysis_div = document.getElementById('valgrind.result1.Report');
    analysis_div.innerHTML=data;
}
updateContentOnceLoaded1();
