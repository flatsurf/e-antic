**Added:**

* Added functions ``renf_elem_addmul`` and ``renf_elem_submul`` to compute ``res += a * b`` and ``res -= a * b`` with fewer temporaries. In the C++ interface, these methods are called ``iaddmul`` and ``isubmul``.
