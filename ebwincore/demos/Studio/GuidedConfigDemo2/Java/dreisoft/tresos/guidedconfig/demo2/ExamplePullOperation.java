/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo2;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractConfigurablePushOperation;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushOperation;

import java.util.ArrayList;
import java.util.List;

/**
 * This is an example for a {@link AbstractPushOperation}.
 *
 * <p>
 * This operation serves the {@link Demo2Backend}. It loads data from the project configuration and stores it into
 * {@link Memento}s.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class ExamplePullOperation extends AbstractConfigurablePushOperation
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
    public ExamplePullOperation()
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
     *        The demo implementation serves the {@link Demo2Backend}.
     *        </p>
     *
     * @param event The push event
     */
    @Override
    public void doInvoke(AbstractPushEvent event)
    {
        if (!(event instanceof ExamplePullEvent))
        {
            return;
        }
        ExamplePullEvent exampleEvent = (ExamplePullEvent)event;

        // obtain reference to data model from the event for reading data
        DCtxt targetContext = exampleEvent.getTargetContext();

        // obtain memento for writing data
        Memento memento = exampleEvent.getSourceMemento();

        List<String> changedMemento = new ArrayList<String>();

        // write values from the data model to the memento
        // the data stored in the memento will be used as values of the GUI widget for the dialog
        memento.setBoolean(IDemo2Constants.MEMENTO_CHECKBOX, targetContext.var.getBool(DCTXT_PATH_BOOL));
        changedMemento.add(IDemo2Constants.MEMENTO_CHECKBOX);
        memento.setInteger(IDemo2Constants.MEMENTO_INT, targetContext.var.getInt(DCTXT_PATH_INT));
        changedMemento.add(IDemo2Constants.MEMENTO_INT);
        memento.setFloat(IDemo2Constants.MEMENTO_FLOAT, targetContext.var.getFloat(DCTXT_PATH_FLOAT));
        changedMemento.add(IDemo2Constants.MEMENTO_FLOAT);
        memento.setString(IDemo2Constants.MEMENTO_STRING, targetContext.var.getString(DCTXT_PATH_STRING));
        changedMemento.add(IDemo2Constants.MEMENTO_STRING);

        exampleEvent.setChangedMementos(changedMemento);
    }

} // ExamplePullOperation
