**Fixed:**

* Fixed deserialization of renf_class from old Python pickles. (#242 sometimes crashed when called from Python since cppyy had trouble with the exceptions being thrown.)
