#include "../../common.h"

#include "../../ds/ds_deque.h"
#include "../objects/php_deque.h"
#include "php_deque_iterator.h"

static void php_ds_deque_iterator_dtor(zend_object_iterator *intern)
{
}

static int php_ds_deque_iterator_valid(zend_object_iterator *iter)
{
    php_ds_deque_iterator_t *iterator = (php_ds_deque_iterator_t *) iter;

    if (iterator->position < iterator->deque->size) {
        return SUCCESS;
    }

    return FAILURE;
}

static zval *php_ds_deque_iterator_get_current_data(zend_object_iterator *iter)
{
    php_ds_deque_iterator_t *iterator = (php_ds_deque_iterator_t *) iter;
    return ds_deque_get(iterator->deque, iterator->position);
}

static void php_ds_deque_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((php_ds_deque_iterator_t *) iter)->position);
}

static void php_ds_deque_iterator_move_forward(zend_object_iterator *iter)
{
    ((php_ds_deque_iterator_t *) iter)->position++;
}

static void php_ds_deque_iterator_rewind(zend_object_iterator *iter)
{
    ((php_ds_deque_iterator_t *) iter)->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    php_ds_deque_iterator_dtor,
    php_ds_deque_iterator_valid,
    php_ds_deque_iterator_get_current_data,
    php_ds_deque_iterator_get_current_key,
    php_ds_deque_iterator_move_forward,
    php_ds_deque_iterator_rewind
};

static zend_object_iterator *create_iterator(ds_deque_t *deque, int by_ref)
{
    php_ds_deque_iterator_t *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(php_ds_deque_iterator_t));
    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs = &iterator_funcs;
    iterator->deque        = deque;
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *php_ds_deque_get_iterator_ex(zend_class_entry *ce, zval *object, int by_ref, ds_deque_t *deque)
{
    return create_iterator(deque, by_ref);
}

zend_object_iterator *php_ds_deque_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    return create_iterator(Z_DS_DEQUE_P(object), by_ref);
}
