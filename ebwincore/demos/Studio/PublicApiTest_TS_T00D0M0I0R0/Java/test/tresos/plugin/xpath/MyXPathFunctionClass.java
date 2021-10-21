package test.tresos.plugin.xpath;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.datamodel2.api.model.xpath.AbstractXPathFunctions;
import dreisoft.tresos.datamodel2.api.model.xpath.CurrentContext;
import dreisoft.tresos.lib2.api.log.APIInvalidOperationException;

/**
 * Class with new XPath functions registered.
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class MyXPathFunctionClass extends AbstractXPathFunctions
{

    /**
     * Unused constructor.
     *
     * @since 2008.b
     */
    public MyXPathFunctionClass()
    {
    }

    /**
     * This function is used in the PublicApiTest_TS_T00D0M0I0R0.xdm to execute an enable check of a node. See the
     * Examples/Float node
     *
     * @since 2008.b
     */
    public static Object MyTestXPathFunction(CurrentContext context, Object o) throws APIInvalidOperationException
    {
        DCtxt node = toDCtxt(o);
        if (node.var.getBool())
        {
            return getReturnValue(context, Boolean.TRUE);
        }
        else
        {
            return getReturnValue(context, Boolean.FALSE);
        }
    }
} // MyXPathFunctionClass

