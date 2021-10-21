package test.tresos.plugin.xpath;

import dreisoft.tresos.datamodel2.api.model.xpath.AbstractXPathFunctions;
import dreisoft.tresos.lib2.api.log.APIInvalidOperationException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

/**
 * Class with new XPath functions registered.
 *
 * Contains functions to store strings within arrays and arrays of strings within arrays. They can e.g. used as follows:
 *
 * <code>
 * array:add('myArray', '1', '2')
 * array:get('myArray', 2)
 * array:add('arrayArray', array:array('myArray'))
 * </code>
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class MyXPathArrayFunctions extends AbstractXPathFunctions
{
    /**
     * stores the simple String-arrays: name --> Vector&lt;String&gt;
     */
    private static HashMap<String, Vector<String>> arrays = new HashMap<String, Vector<String>>();
    /**
     * stores the array of arrays: name --> Vector&lt;String&gt; --> Vecto&lt;String&gt;
     */
    private static HashMap<String, Vector<Vector<String>>> aarrays = new HashMap<String, Vector<Vector<String>>>();

    /**
     * Unused constructor.
     *
     * @since 2008.b
     */
    public MyXPathArrayFunctions()
    {
    }

    /**
     * Adds the given value to the array with the given name. If the array does not already exists, it is created.
     */
    public static boolean add(Object name, Object value1) throws APIInvalidOperationException
    {
        return arrayAdd(toString(name), toString(value1));
    }

    /**
     * Adds the given values to the array with the given name. If the array does not already exists, it is created.
     */
    public static boolean add(Object name, Object value1, Object value2) throws APIInvalidOperationException
    {
        return arrayAdd(toString(name), toString(value1), toString(value2));
    }

    /**
     * Adds the given values to the array with the given name. If the array does not already exists, it is created.
     */
    public static boolean add(Object name, Object value1, Object value2, Object value3)
        throws APIInvalidOperationException
    {
        return arrayAdd(toString(name), toString(value1), toString(value2), toString(value3));
    }

    /**
     * Adds the given value to the array with the given name. If the array does not already exists, it is created.
     */
    private static boolean arrayAdd(String name, String... values) throws APIInvalidOperationException
    {
        if ((name == null) || (values == null))
        {
            return false;
        }
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            array = new Vector<String>();
            arrays.put(name, array);
        }
        boolean ret = true;
        for (String value : values)
        {
            ret &= array.add(toString(value));
        }
        return ret;
    }

    /**
     * Adds all values of the array with name2 to the array with name1. If the array "name1" does not already exists, it
     * is created.
     */
    public static boolean addAll(Object name1, Object name2) throws APIInvalidOperationException
    {
        if ((name1 == null) || (name2 == null))
        {
            return false;
        }
        Vector<String> array1 = arrays.get(name1);
        Vector<String> array2 = arrays.get(name2);
        if (array2 == null)
        {
            return true;
        }
        if (array1 == null)
        {
            array1 = new Vector<String>();
            arrays.put(toString(name1), array1);
        }
        return array1.addAll(array2);
    }

    /**
     * @return the size of the array with the given name. 0 if it does not exists.
     */
    public static int size(Object name)
    {
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return 0;
        }
        return array.size();
    }

    /**
     * @return the array with the given name. An empty array is returned, if there is no existing one.
     */
    public static List<String> array(Object name)
    {
        if (name == null)
        {
            return new ArrayList<String>();
        }
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return new ArrayList<String>();
        }
        return array;
    }

    /**
     * @return the index'th element from the array with the given name. An empty string is returned if the array does
     *         not exists or the index is out of range. The index starts with 1!!!
     */
    public static String get(Object name, Object index) throws APIInvalidOperationException
    {
        int nr = (int)toInt(index);
        if ((name == null) || (nr < 1))
        {
            return "";
        }
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return "";
        }
        if (array.size() < nr)
        {
            return "";
        }
        return array.get(nr - 1);
    }

    /**
     * @return a string representation of the array with the given name. An empty string is returned if the array does
     *         not already exists
     */
    public static String print(Object name)
    {
        if (name == null)
        {
            return "";
        }
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return "";
        }
        String ret = "";
        for (String elem : array)
        {
            ret += " " + elem;
        }
        if (ret.startsWith(" "))
        {
            ret = ret.substring(1);
        }
        return ret;
    }

    /**
     * @return true, if the given value already exists within the array with the given name - false in any other case
     */
    public static boolean contains(Object name, Object value) throws APIInvalidOperationException
    {
        if (name == null)
        {
            return false;
        }
        String strVal = toString(value);
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return false;
        }
        return array.contains(strVal);
    }

    /**
     * removes all elements from the array with the given name
     */
    public static void clear(String name)
    {
        if (name == null)
        {
            return;
        }
        Vector<String> array = arrays.get(name);
        if (array == null)
        {
            return;
        }
        array.clear();
    }

    // #######################
    // Array within an Array #
    // #######################

    /**
     * adds the given values as array to the array with the given name. If the array with the given name does not
     * already exists, it is created.
     *
     * @param values1 a list of string-values to store
     */
    public static boolean addArray(Object name, Object values1) throws APIInvalidOperationException
    {
        return arrayAddArray(toString(name), toValues(values1));
    }

    /**
     * adds the given values as array to the array with the given name. If the array with the given name does not
     * already exists, it is created.
     *
     * @param values1 a list of string-values to store
     * @param values2 a list of string-values to store
     */
    public static boolean addArray(Object name, Object values1, Object values2) throws APIInvalidOperationException
    {
        return arrayAddArray(toString(name), toValues(values1), toValues(values2));
    }

    /**
     * adds the given values as array to the array with the given name. If the array with the given name does not
     * already exists, it is created.
     *
     * @param values1 a list of string-values to store
     * @param values2 a list of string-values to store
     * @param values3 a list of string-values to store
     */
    public static boolean addArray(Object name, Object values1, Object values2, Object values3)
        throws APIInvalidOperationException
    {
        return arrayAddArray(toString(name), toValues(values1), toValues(values2), toValues(values3));
    }

    /**
     * adds the given values as array to the array with the given name. If the array with the given name does not
     * already exists, it is created.
     */
    private static boolean arrayAddArray(String name, Object[]... lists) throws APIInvalidOperationException
    {
        if ((name == null) || (lists == null))
        {
            return false;
        }
        Vector<Vector<String>> array = aarrays.get(name);
        if (array == null)
        {
            array = new Vector<Vector<String>>();
            aarrays.put(name, array);
        }
        Vector<String> newList = new Vector<String>();
        boolean ret = true;
        for (Object[] list : lists)
        {
            for (Object value : list)
            {
                ret &= newList.add(toString(value));
            }
        }
        ret &= array.add(newList);
        return ret;
    }

    /**
     * @return the array with the given index (starting at 1) from the array of the given name. An empty array is
     *         returned, if the named array does not exist or the index is out of range
     */
    public static List<String> getArray(Object name, Object index) throws APIInvalidOperationException
    {
        int nr = (int)toInt(index);
        if ((name == null) || (nr < 1))
        {
            return new ArrayList<String>();
        }
        Vector<Vector<String>> array = aarrays.get(name);
        if (array == null)
        {
            return new ArrayList<String>();
        }
        if (array.size() < nr)
        {
            return new ArrayList<String>();
        }
        return array.get(nr - 1);
    }

    /**
     * @return a string-representation of the array with the given name
     */
    public static String printArray(Object name)
    {
        if (name == null)
        {
            return "";
        }
        Vector<Vector<String>> outer = aarrays.get(name);
        if (outer == null)
        {
            return "";
        }
        String ret = "";
        for (Vector<String> inner : outer)
        {
            for (String elem : inner)
            {
                ret += " " + elem;
            }
        }
        if (ret.startsWith(" "))
        {
            ret = ret.substring(1);
        }
        return ret;
    }

    /**
     * removes all elements from the array with the given name
     */
    public static void clearArray(Object name)
    {
        if (name == null)
        {
            return;
        }
        Vector<Vector<String>> array = aarrays.get(name);
        if (array == null)
        {
            return;
        }
        array.clear();
    }

} // MyXPathFunctionClass

