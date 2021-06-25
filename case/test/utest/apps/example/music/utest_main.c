/********************************************************************************
 *                        USDK130
 *                     unit test example
 *
 *                (c) Copyright 2003-2008, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: utest_main.c
 * By  : jiancailiu
 * Version: 1> v1.00     first version     2008-11-5 15:03
 *******************************************************************************
 */
#include "demo_utest.h"
#include "assert.h"

/* export the cases and their init/clean func from file name_FuncTester.c */
extern CU_TestInfo test_value_func[];
extern int suite_testvalue_init(void);
extern int suite_testvalue_clean(void);

static CU_SuiteInfo suites[] = {
    /* func test */
    { "test_value_operate", test_value_func,  suite_testvalue_init, suite_testvalue_clean },

	CU_SUITE_INFO_NULL,
};


void AddTests(void)
{
  assert(NULL != CU_get_registry());
  assert(!CU_is_test_running());

	/* Register suites. */
	if (CU_register_suites(suites) != CUE_SUCCESS) {
		printf("suite registration failed - %s\n",
			CU_get_error_msg());
		cunit_exit(EXIT_FAILURE);
	}
}

void utest_main(void)
{
    void *fptr;

    printf("config_task, LINE %d \n", __LINE__);
    //assert(fptr);

#ifndef SUPPORT_API

   // Person_alloc = dlsym(fptr, "Person_alloc");
   // Person_init = dlsym(fptr, "Person_init");
  //  Person_initWithName = dlsym(fptr, "Person_initWithName");
  //  Person_personWithName = dlsym(fptr, "Person_personWithName");
   // Person_dealloc = dlsym(fptr, "Person_dealloc");
   // Person_fullName = dlsym(fptr, "Person_fullName");
   // Person_firstName = dlsym(fptr, "Person_firstName");
   // Person_lastName = dlsym(fptr, "Person_lastName");
   // Person_setFullName = dlsym(fptr, "Person_setFullName");
   // Person_setFirstName = dlsym(fptr, "Person_setFirstName");
   // Person_setLastName = dlsym(fptr, "Person_setLastName");
   //test_testfunc = dlsym("demo.exe", "test_testfunc");   //???
   
#ifdef __COVTEST__
    func_cov = dlsym(fptr, "func_cov");
#endif

#endif

    int retv;

    retv = cunit_init();
    if (retv == 0)
    {
        if (CU_initialize_registry(NULL, NULL))
        {
            printf("\nInitialization of Test Registry failed.");
        }
        else
        {
            CU_registry_setname("exampleTest");
            AddTests();

		      printf("  all tests have add \n");
            CU_set_error_action(CUEA_IGNORE);
            printf("\nTests completed with return value %d.\n", CU_remote_run_tests());
            CU_cleanup_registry();
        }
    }
    else
    {
        printf("\nAbort testing or Error happens.");
    }

    //dlclose(fptr);
   // exit();
    return;
}

