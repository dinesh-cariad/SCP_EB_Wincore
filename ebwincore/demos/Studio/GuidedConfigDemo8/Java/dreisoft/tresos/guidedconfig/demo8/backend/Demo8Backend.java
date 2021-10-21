/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo8.backend;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.PushService;
import dreisoft.tresos.guidedconfig.demo8.IConstants;
import dreisoft.tresos.guidedconfig.demo8.generated.operationstatus.MyGuidedConfigOperationStatus;
import dreisoft.tresos.guidedconfig.demo8.memento.Demo8MementoFactory;
import dreisoft.tresos.guidedconfig.demo8.pushservice.DataTransferEvent;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.util.ArrayList;
import java.util.List;

/**
 * This demo features a simple editor that manipulates the data model.
 */
public class Demo8Backend extends AbstractBackend
{

    //
    // defines

    /**
     * The ID of the "prefix" widget in the GUI.
     */
    private static final String WIDGET_ID_PREFIX_TEXT = "PrefixText";

    /**
     * The ID of the "preview" widget in the GUI.
     */
    private static final String WIDGET_ID_PREVIEW_TEXT = "PreviewText";

    /**
     * Path to the MODULE-DEF of the demo module.
     */
    private static final String MODULE_SCHEMA_PATH = "/AUTOSAR/TOP-LEVEL-PACKAGES/TS_T00D0M0I0R0/ELEMENTS/GCDemo8";

    @Override
    public void doShowPage(AbstractPage page)
    {
        // create the push event for retrieving the data from the data model and
        // to write it into the mementos
        DataTransferEvent event = createPushEvent(DataTransferEvent.EVENT_DIRECTION_PULL);

        // sends the event via the push service; because of the direction of
        // this event, this will cause
        // Demo8PullOperation to be invoked
        PushService.getInstance().callSync(event, false);
    }

    @Override
    public List<APIOperationStatus> doRun()
    {
        // create the push event for writing the data from the memento to the
        // data model
        DataTransferEvent event = createPushEvent(DataTransferEvent.EVENT_DIRECTION_PUSH);

        // sends the event via the push service; because of the direction of
        // this event, this will cause
        // Demo8PushOperation to be invoked
        PushService.getInstance().callSync(event, false);

        return event.getMessages();
    }

    /**
     * Creates a push event and sets the required variables.
     *
     * @param direction the direction of the event, either {@link DataTransferEvent#EVENT_DIRECTION_PULL} or
     *        {@link DataTransferEvent#EVENT_DIRECTION_PUSH}
     * @return the push event, ready for use
     */
    private DataTransferEvent createPushEvent(String direction)
    {
        // create a new instance of the event
        DataTransferEvent event = new DataTransferEvent();

        // obtain the module configuration on which the event will operate
        DCtxt context = getDCtxt();
        event.setModuleConfigDCtxt(context);
        // set the memento for the event
        event.setMemento(getMemento());
        // set the direction of this event; DataTransferEvent can be used in
        // both directions
        event.addCondition(DataTransferEvent.EVENT_DIRECTION, direction);
        return event;
    }

    /**
     * @return the module configuration DCtxt which is currently opened in the editor
     * @since 2010.a
     */
    private DCtxt getDCtxt()
    {
        // get the current ECUConfigContext instance; please note: while the
        // wizard is open, getContexts() will always
        // return the same result and the contexts will not change
        ECUConfigContext ecuConfigContext = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);

        // project context
        DCtxt projectContext = ecuConfigContext.getDCtxt();
        // get all module contexts matching the schema path
        List<DCtxt> moduleContexts = projectContext.getFromSchema(MODULE_SCHEMA_PATH);
        if ((moduleContexts == null) || (moduleContexts.size() == 0))
        {
            // the trigger extension in the plugin.xml file requires that a
            // module of type "GCDemo8" is present; so, if
            // we do not get a valid DCtxt for the path MODULE_SCHEMA_PATH here,
            // something went severely wrong and we
            // should throw an exception here
            throw new IllegalArgumentException("Cannot retrieve data context from path " + MODULE_SCHEMA_PATH);
        }
        // since the module does not allow multiple module configurations
        // (controlled via its extension in the
        // plugin.xml file), the first one is always the correct one
        DCtxt context = moduleContexts.get(0);
        return context;
    }

    @Override
    public void doUpdateWidgetEnablement()
    {
        // the preview text shall never be editable by the user
        changeWidgetEnablement(WIDGET_ID_PREVIEW_TEXT, false);

        Memento rootMemento = getMemento();
        boolean addPrefix = rootMemento.getBoolean(IConstants.MEMENTO_ADD_PREFIX);
        changeWidgetEnablement(WIDGET_ID_PREFIX_TEXT, addPrefix);
    }

    @Override
    public List<ValidationResult> doValidate()
    {
        List<ValidationResult> result = new ArrayList<ValidationResult>();
        Memento rootMemento = getMemento();
        // retrieve current values
        boolean addPrefix = rootMemento.getBoolean(IConstants.MEMENTO_ADD_PREFIX);
        String prefixText = rootMemento.getString(IConstants.MEMENTO_PREFIX_TEXT);
        // display error if "addPrefix" is set without MEMENTO_PREFIX_TEXT
        // having a useful value
        if (addPrefix && ((prefixText == null) || "".equals(prefixText)))
        {
            result.add(new ValidationResult(
                IConstants.MEMENTO_PREFIX_TEXT,
                MyGuidedConfigOperationStatus.ERROR_EMPTY_PREFIX()));
        }
        return result;
    }

    @Override
    public void doHandleUpdatedData(List<String> affectedPaths)
    {
        super.doHandleUpdatedData(affectedPaths);

        boolean fullUpdate = affectedPaths.contains("/");

        if (fullUpdate
            || affectedPaths.contains(IConstants.MEMENTO_ADD_PREFIX)
            || affectedPaths.contains(IConstants.MEMENTO_PREFIX_TEXT))
        {
            // if the prefix changed, we need to update the preview
            updatePreviewMemento();
        }
    }

    /**
     * Updates the preview memento based on data retrieved from other mementos and issues a GUI update.
     */
    private void updatePreviewMemento()
    {
        Memento rootMemento = getMemento();
        // retrieve memento attribute values from the root memento
        boolean addPrefix = rootMemento.getBoolean(IConstants.MEMENTO_ADD_PREFIX);
        String prefixText = rootMemento.getString(IConstants.MEMENTO_PREFIX_TEXT);
        String originalText = rootMemento.getString(IConstants.MEMENTO_ORIGINAL_TEXT);

        getMementoOperationHandler().beginTransaction("Demo8Transaction");

        // update preview, depending on the addPrefix setting
        if (addPrefix)
        {
            getMemento().setString(IConstants.MEMENTO_PREVIEW_TEXT, prefixText + originalText);
        }
        else
        {
            getMemento().setString(IConstants.MEMENTO_PREVIEW_TEXT, originalText);
        }
        getMementoOperationHandler().commitTransaction();

        // since we just updated the data representation
        // (IConstants.MEMENTO_PREVIEW_TEXT), we need to trigger an update
        // of the GUI
        updateView();
    }

    @Override
    public AbstractMementoFactory doGetMementoFactory()
    {
        // overwrite the default implementation so that we are able to return
        // our custom memento factory
        return Demo8MementoFactory.INSTANCE;
    }

} // Demo8Backend

