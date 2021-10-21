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
 * Reads data from the memento and writes it to the data model.
 *
 * This push operation implementation is used to read data from the wizard's {@link Memento} and write it to the data
 * model {@link DCtxt} instance. Depending on the point of view, this class could be considered a "push" operation.
 */
public class Demo8PushOperation extends AbstractConfigurablePushOperation
{

    @Override
    public void doInvoke(AbstractPushEvent event)
    {
        // as specified in the plugin.xml file, we expect the event to be of type DataTransferEvent
        if (!(event instanceof DataTransferEvent))
        {
            return;
        }
        DataTransferEvent exampleEvent = (DataTransferEvent)event;

        // obtain reference to data model from the event for reading data
        DCtxt targetContext = exampleEvent.getModuleConfigDCtxt();

        // obtain memento for writing data
        Memento memento = exampleEvent.getMemento();

        String value = memento.getString(IConstants.MEMENTO_PREVIEW_TEXT);
        targetContext.var.setString(IConstants.DCTXT_PATH_STRING, value);
    }

} // Demo8PushOperation
