/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo7;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractConfigurablePushOperation;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushOperation;

import java.util.ArrayList;
import java.util.List;

/**
 * This is an example for a {@link AbstractPushOperation}.
 *
 * <p>
 * This operation serves the {@link Demo7Backend} to write to the project configuration.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class ExampleAutoconfigureOperation extends AbstractConfigurablePushOperation
{

    // constants for paths into DCtxt
    public static final String DCTXT_PATH_BOOL = "ExamplesCommon/Bool";
    public static final String DCTXT_PATH_INT = "ExamplesCommon/Int";
    public static final String DCTXT_PATH_STRING = "ExamplesCommon/String";
    public static final String DCTXT_PATH_FLOAT = "ExamplesCommon/Float";

    /**
     * Default constructor.
     *
     * <p>
     * Normally nothing to do here.
     * </p>
     */
    public ExampleAutoconfigureOperation()
    {
    }

    /**
     * Invokes the push operation.
     *
     * <p>
     * This method does the actual work push transferring data between the
     *
     * @{link AbstractPushEvent} and the project.
     *        </p>
     *
     *        <p>
     *        The demo implementation serves the {@link Demo7Backend}.
     *        </p>
     *
     * @param event The push event
     */
    @Override
    public void doInvoke(AbstractPushEvent event)
    {
        if (!(event instanceof ExampleAutoconfigureEvent))
        {
            return;
        }
        ExampleAutoconfigureEvent exampleEvent = (ExampleAutoconfigureEvent)event;
        DCtxt targetContext = exampleEvent.getTargetContext();

        List<DCtxt> changedNodes = new ArrayList<DCtxt>();
        setBool(changedNodes, targetContext, DCTXT_PATH_BOOL, false);
        setInt(changedNodes, targetContext, DCTXT_PATH_INT, 23);
        setString(changedNodes, targetContext, DCTXT_PATH_STRING, "");
        setFloat(changedNodes, targetContext, DCTXT_PATH_FLOAT, 23.5);

        exampleEvent.setChangedNodes(changedNodes);
    }

    /**
     * Change a String variable.
     *
     * @param changedNodes The list of changed nodes
     * @param ctxt The target context
     * @param path The path in the target context
     * @param value the new value
     */
    private void setString(List<DCtxt> changedNodes, DCtxt ctxt, String path, String value)
    {
        String oldValue = ctxt.var.getString(path);
        boolean changed;
        if ((value == null) || (oldValue == null))
        {
            changed = (value == oldValue);
        }
        else
        {
            changed = !value.equals(oldValue);
        }

        if (changed)
        {
            ctxt.var.setString(path, value);
            changedNodes.add(ctxt.get(path));
        }
    }

    /**
     * Change a Float variable.
     *
     * @param changedNodes The list of changed nodes
     * @param ctxt The target context
     * @param path The path in the target context
     * @param value the new value
     */
    private void setFloat(List<DCtxt> changedNodes, DCtxt ctxt, String path, double value)
    {
        if (ctxt.var.getFloat(path) != value)
        {
            ctxt.var.setFloat(path, value);
            changedNodes.add(ctxt.get(path));
        }
    }

    /**
     * Change a Int variable.
     *
     * @param changedNodes The list of changed nodes
     * @param ctxt The target context
     * @param path The path in the target context
     * @param value the new value
     */
    private void setInt(List<DCtxt> changedNodes, DCtxt ctxt, String path, long value)
    {
        if (ctxt.var.getInt(path) != value)
        {
            ctxt.var.setInt(path, value);
            changedNodes.add(ctxt.get(path));
        }
    }

    /**
     * Change a Bool variable.
     *
     * @param changedNodes The list of changed nodes
     * @param ctxt The target context
     * @param path The path in the target context
     * @param value the new value
     */
    private void setBool(List<DCtxt> changedNodes, DCtxt ctxt, String path, boolean value)
    {
        if (ctxt.var.getBool(path) != value)
        {
            ctxt.var.setBool(path, value);
            changedNodes.add(ctxt.get(path));
        }
    }

} // ExampleAutoconfigureOperation
