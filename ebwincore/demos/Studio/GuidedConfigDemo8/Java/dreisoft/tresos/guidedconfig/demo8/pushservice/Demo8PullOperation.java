/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo8.pushservice;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractConfigurablePushOperation;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.demo8.IConstants;

/**
 * Reads data from the data model and writes it to the memento.
 *
 * This push operation implementation is used to read data from the data model {@link DCtxt} and write it to the
 * wizard's {@link Memento} instance. Depending on the point of view, this class could be considered a "pull" operation.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 */
public class Demo8PullOperation extends AbstractConfigurablePushOperation
{

    @Override
    public void doInvoke(AbstractPushEvent e)
    {
        // as specified in the plugin.xml file, we expect the event to be of type DataTransferEvent
        if (!(e instanceof DataTransferEvent))
        {
            return;
        }
        DataTransferEvent event = (DataTransferEvent)e;

        // obtain reference to data model from the event for reading data
        DCtxt context = event.getModuleConfigDCtxt();

        // obtain memento for writing data
        Memento memento = event.getMemento();

        // write values from the data model to the memento
        // the data stored in the memento will be used as values of the GUI widget for the dialog
        String value = context.var.getString(IConstants.DCTXT_PATH_STRING);
        if (value == null)
        {
            value = "";
        }
        memento.setString(IConstants.MEMENTO_ORIGINAL_TEXT, value);
    }

} // Demo8PullOperation
