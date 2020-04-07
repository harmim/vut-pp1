# Experiments on Static Analysis Using Atomer

*Based on dynamic analysis of contracts -
https://pajda.fit.vutbr.cz/smrcka/contracts-first-experiments.*

The experiments structure:
- Each directory contains a single experiment. There are the following files:
  * `..._pass.cc` - a program that should pass the analysis, i.e., a program
    without atomicity violations
  * `..._fail.cc` - a program that should fail the analysis, i.e., a program
    with atomicity violations
  * `atomic-sets_pass` - atomic sets derived from `..._pass.cc`
  * `bugs_pass.txt` - a bugs report of the analysis of `..._pass.cc`
  * `bugs_fail.txt` - a bugs report of the analysis of `..._fail.cc`
