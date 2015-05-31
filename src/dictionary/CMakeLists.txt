# dodajemy bibliotekę dictionary, stworzoną na podstawie pliku dictionary.c
# biblioteka będzie dołączana statycznie (czyli przez linkowanie pliku .o)

add_library (dictionary dictionary.c word_list.c vector.c trie.c)

if (CMOCKA)
    # dodajemy plik wykonywalny z testem    
    add_executable (word_list_test word_list.c word_list_test.c)
    add_executable (vector_test vector.c vector_test.c)
    add_executable (dictionary_test dictionary.c dictionary_test.c)

    # i linkujemy go z biblioteką do testowania
    target_link_libraries (word_list_test ${CMOCKA})
    target_link_libraries (vector_test ${CMOCKA})
    target_link_libraries (dictionary_test ${CMOCKA})

    # wreszcie deklarujemy, że to test
    add_test (word_list_unit_test word_list_test)
    add_test (vector_unit_test vector_test)
    add_test (dictionary_unit_test dictionary_test) 

endif (CMOCKA)

