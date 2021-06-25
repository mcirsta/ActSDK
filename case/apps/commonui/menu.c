/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menu.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-21        v1.0               build this file 
 ********************************************************************************/
/*!
* \file     menu.c
* \brief    menu data operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/04/21
*******************************************************************************/
    
#include "include_case.h"


#include "menu_private.h"

static bool _insert_item( menu_t *menu, int cur_index, menuitem_t *item );
static bool _remove_item( menu_t *menu, int cur_index );


/* menu item data interface */
menuitem_t* menuitem_create( void )
{
    menuitem_t *item;

    item = malloc( sizeof(menuitem_t) );
    
    if ( item != NULL )
    {
        memset( item, 0, sizeof(menuitem_t) );        
    }
    else
    {
        print_err( "create menu item fail!" );
    }

    return item;
}

bool menuitem_delete( menuitem_t *item )
{
    if ( item == NULL )
    {
        return false;
    }
    else
    {
        free( item );
        return true;
    }
}

/* menu data interface */
menu_t* menu_create( void )
{
    menu_t *menu;

    menu = malloc( sizeof(menu_t) );
    
    if ( menu != NULL )
    {
        memset( menu, 0, sizeof(menu_t) );
    }
    else
    {
        print_err( "create menu fail!" );
    }

    return menu;
}

bool menu_delete( menu_t *menu )
{
    menuitem_inner_t* it;
    menuitem_inner_t* next;
    
    if ( menu == NULL )
    {
        return false;
    }
    else
    {
        //free item data
        it = menu->head;
        while( it != NULL )
        {
            next = it->next;
            free( it );
            it = next;
        }
        
        //free menu data
        free( menu );
        
        return true;
    }
}

/* get inner item in the list */
static menuitem_inner_t* menu_get_inner_item( menu_t *menu, int cur_index )
{
    menuitem_inner_t* it;
    int num;

    if ( ( menu == NULL ) || ( cur_index < 0 ) )
    {
        return NULL;
    }
    else
    {
        it = menu->head;
    }
    
    for ( num=0; num < cur_index; num++ )
    {
        if ( it != NULL )
        {
            it = it->next;
        }
        else    //item is last
        {
            break;
        }
    }

    return it;    
}

bool menu_set_cur_index( menu_t *menu, int current )
{
    if ( menu == NULL )
    {
        return false;
    }
    
    if ( ( current >= 0 ) && ( current < menu->total ) )
    {
        menu->current = current;
        return true;
    }
    else
    {
        return false;
    }
}

int menu_get_cur_index( menu_t *menu )
{
    if ( menu == NULL )
    {
        return -1;
    }
    else
    {
        return menu->current;
    }    
}

bool menu_set_offset( menu_t *menu, int offset )
{
    if ( menu == NULL )
    {
        return false;
    }
    
    if ( ( offset >= 0 ) && ( offset < menu->total ) )
    {
        menu->offset = offset;
        return true;
    }
    else
    {
        return false;
    }
}

int menu_get_offset( menu_t *menu )
{
    if ( menu == NULL )
    {
        return -1;
    }
    else
    {
        return menu->offset;
    }    
}

int menu_get_total( menu_t *menu )
{
    if ( menu == NULL )
    {
        return -1;
    }
    else
    {
        return menu->total;
    }    
}

menuitem_t* menu_get_cur_item( menu_t *menu )
{
    if ( menu == NULL )
    {
        return NULL;
    }
    else
    {
        return menu_get_item( menu, menu->current );
    }     
}

menuitem_t* menu_get_item( menu_t *menu, int cur_index )
{
    menuitem_inner_t* it;

    it = menu_get_inner_item( menu, cur_index );
    
    if ( it != NULL )
    {
        return &it->data;
    }
    else
    {
        return NULL;
    }
}

bool menu_set_item( menu_t *menu, int cur_index, menuitem_t *item )
{
    menuitem_inner_t* it;

    it = menu_get_inner_item( menu, cur_index );
    
    if ( ( it != NULL ) && ( item != NULL ) )
    {
        it->data = ( *item );
        return true;
    }
    else
    {
        return false;
    }
}

static bool _insert_item( menu_t *menu, int cur_index, menuitem_t *item )
{
    menuitem_inner_t* p = menu->head;
    menuitem_inner_t* it;
    int j = 0;

    //malloc and copy item data
    it = malloc( sizeof(menuitem_inner_t) );
    if ( it == NULL )
    {
        print_err( "malloc failed!" );
        return false;
    }
    else
    {
        it->data = (*item);
        it->next = NULL;
    }
    
    //search insert position    
    if ( p == NULL )    //list is null
    {
        menu->head = it;
        return true; 
    }
    else if ( cur_index == 0 )    //insert at head
    {
        it->next = p;
        menu->head = it;
        return true; 
    }
    else
    {
        while ( ( j < (cur_index-1) ) && ( p != NULL ) )
        { 
            j++; 
            p = p->next; 
        } 
    }
    
    //insert item    
    if ( p == NULL ) 
    {
        free( it );
        print_err("index=%d", cur_index);
        return false; 
    }
    else 
    { 
        it->next = p->next; 
        p->next = it; 
        return true; 
    }    
}

bool menu_insert_item( menu_t *menu, int cur_index, menuitem_t *item )
{
    bool ret;

    if ( menu == NULL )
    {
        return false;
    }
    
    //insert item content
    ret = _insert_item( menu, cur_index, item );

    //modify current, total
    if ( ret == true )
    {
        menu->total ++;
        
        if ( cur_index <= menu->current )
        {
            menu->current ++;            
        }
        
        if ( ( menu->total > 0 ) && ( menu->current > (menu->total - 1) ) )
        {
            menu->current = menu->total - 1;        
        }
    }
    
    return ret;
}

bool menu_prepend_item( menu_t *menu, menuitem_t *item )
{    
    if ( menu == NULL )
    {
        return false;
    }
    else
    {
        return menu_insert_item( menu, 0, item );
    }    
}

bool menu_append_item( menu_t *menu, menuitem_t *item )
{
    if ( menu == NULL )
    {
        return false;
    }
    else
    {
        return menu_insert_item( menu, menu->total, item );
    }    
}

bool menu_append_items( menu_t *menu, menuitem_t *items, int num )
{
    int cur_index;
    bool ret = false;
    
    for ( cur_index = 0; cur_index < num; cur_index ++ )
    {
        ret = menu_append_item( menu, &items[cur_index] );
    }
    
    return ret;
}

static bool _remove_item( menu_t *menu, int cur_index )
{
    menuitem_inner_t* p = menu->head;
    menuitem_inner_t* it;
    int j = 0;
    
    if ( cur_index == 0 )    //remove head
    {
        menu->head = p->next;
        free( p );
        return true; 
    }
    else
    {
        while ( (j < (cur_index-1) ) && ( p != NULL ) )
        { 
            j++; 
            p = p->next; 
        } 
    }
    
    if ( p == NULL)                /*未找到第i-1个结点*/ 
    { 
        print_err();
        return false; 
    }
    else                        /*找到第i-1个结点*p*/ 
    { 
        it = p->next;                /*it指向要删除的结点*/ 
        p->next = it->next;        /*从单链表中删除*it结点*/ 
        free( it );                /*释放*it结点*/        
        return true; 
    } 
}

bool menu_remove_item( menu_t *menu, int cur_index )
{
    bool ret;

    if ( menu == NULL )
    {
        return false;
    }
    
    //remove item content
    ret = _remove_item( menu, cur_index );

    //modify current, total
    if ( ret == true )
    {
        menu->total --;
        
        if ( cur_index < menu->current )
        {
            menu->current --;        
        }
        
        if ( ( menu->total > 0 ) && ( menu->current > (menu->total - 1) ) )
        {
            menu->current = menu->total - 1;        
        }
    }
    
    return ret;    
}

bool menu_remove_first_item( menu_t *menu )
{
    if ( menu == NULL )
    {
        return false;
    }
    else
    {
        return menu_remove_item( menu, 0 );
    }  
}

bool menu_remove_last_item( menu_t *menu )
{
    if ( menu == NULL )
    {
        return false;
    }
    else
    {
        return menu_remove_item( menu, menu->total-1 );
    }    
}

bool menu_remove_all_items( menu_t *menu )
{
    int total;
    int times;
    bool ret = false;

    if ( menu == NULL )
    {
        return false;
    }
    
    total = menu->total;
    
    for ( times = 0; times < total; times ++ )
    {
        ret = menu_remove_first_item( menu );
    }
    
    return ret;
}

/* advanced interface */
bool menu_set_global_offset( menu_t *menu, int offset )
{
    if ( menu == NULL )
    {
        return false;
    }
    else
    {  
        menu->global_offset = offset;
        return true;
    }  
}

int menu_get_global_offset( menu_t *menu )
{
    if ( menu == NULL )
    {
        return -1;
    }
    else
    {  
        return menu->global_offset;
    }  
}


bool menu_set_global_size( menu_t *menu, int size )
{
    if ( menu == NULL )
    {
        return false;
    }
    else
    {  
        menu->global_size = size;
        return true;
    }  
}

int menu_get_global_size( menu_t *menu )
{
    if ( menu == NULL )
    {
        return -1;
    }
    else
    {  
        return menu->global_size;
    }  
}

