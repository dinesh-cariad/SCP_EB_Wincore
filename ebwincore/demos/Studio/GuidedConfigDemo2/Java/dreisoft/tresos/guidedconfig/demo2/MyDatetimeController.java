/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo2;

import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.gui.databinding.MementoOperationHandler;
import dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController;
import dreisoft.tresos.guidedconfig.api.memento.Memento;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * This is a sample for a custom widget managed by the wizard.
 *
 * <p>
 * The MyDatetime widget is used by the {@link Demo2Page}.
 * </p>
 *
 * <p>
 * This is the controller class for the {@link MyDatetime} widget. It is responsible to write the data to the
 * {@link Memento} and read from it. This class knows the structure of the Memento for the Datetime widget.
 * </p>
 *
 * <p>
 * The Datetime widget uses a memento with three attributes to store its data:
 * </p>
 *
 * <pre>
 * DatetimeMemento       - The memento (selected by a mementoId)
 * DatetimeMemento/year  - Attribute to store the year
 * DatetimeMemento/month - Attribute to store the month
 * DatetimeMemento/day   - Attribute to store the day
 * </pre>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class MyDatetimeController extends AbstractWidgetController
{

    //
    // defines

    /**
     * Constant for storing the attribute year into the memento
     */
    public static final String MEMENTO_ATTRIBUTE_YEAR = "year";

    /**
     * Constant for storing the attribute month into the memento
     */
    public static final String MEMENTO_ATTRIBUTE_MONTH = "month";

    /**
     * Constant for storing the attribute day into the memento
     */
    public static final String MEMENTO_ATTRIBUTE_DAY = "day";

    //
    // private members

    /**
     * The GUI to control
     */
    private MyDatetime m_element = null;

    /**
     * The mementoId of this control
     */
    private String m_mementoId;

    /**
     * Constructor for the controller class. It is also used to connect the widget to this controller.
     *
     * @param dateTime The {@link MyDatetime} widget to control
     * @param mementoId The mementoId which is used for saving and reading data
     */
    public MyDatetimeController(MyDatetime dateTime, String mementoId)
    {
        m_element = dateTime;
        m_mementoId = mementoId;
        dateTime.setController(this);
    }

    /**
     * Query the Widget-Ids of the widgets that are managed by this controller.
     *
     * <p>
     * The WidgetId of the Datetime widget is managed by the {@link MyDatetime} class.
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doGetWidgetIds()
     */
    @Override
    public List<String> doGetWidgetIds()
    {
        ArrayList<String> widgetIds = new ArrayList<String>();
        widgetIds.add(m_element.getWidgetId());
        return widgetIds;
    }

    /**
     * Checks if the controller manages the given widget.
     *
     * <p>
     * This methods is used by navigation events to find the target widget
     * </p>
     *
     * @param widgetId The widget in question
     * @return true if the controller mamanges this widget
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doCanShowWidget(java.lang.String)
     */
    @Override
    public boolean doCanShowWidget(String widgetId)
    {
        // check if the widgetId corresponds to the widgetId of this Datetime widget
        return (widgetId != null) && widgetId.equals(m_element.getWidgetId());
    }

    /**
     * Checks if this controller is responsible for the given memento.
     *
     * @param mementoPath
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doCanShowMemento(java.lang.String)
     */
    @Override
    public boolean doCanShowMemento(String mementoPath)
    {
        // if it is root, then all widgets shall be updated
        if (mementoPath.equals("/"))
        {
            return true;
        }
        // if the given mementoPath is on the same branch of the memento tree
        if (getMemento().getMementoPath(mementoPath, true).contains(getWidgetMemento().getPath()))
        {
            return true;
        }
        return super.doCanShowMemento(mementoPath);
    }

    /**
     * This method is called by the guided config engine to dispatch navigation events (set focus) to widgets managed by
     * this controller.
     *
     * <p>
     * Dispatches the event to the widget.
     * </p>
     *
     * <p>
     * BEWARE: Scroll to this widget is currently not supported.
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doNavigate(java.lang.String,
     *      java.lang.String)
     */
    @Override
    public void doNavigate(String widgetId, String path)
    {
        if ((m_element != null) && doCanShowWidget(widgetId))
        {
            m_element.setFocus();
        }
    }

    /**
     * Called to load the metadata like width, shown text format etc. of this widget.
     *
     * <p>
     * The handed in memento is exclusively created to store metadata for a specific widget managed by this controller.
     * The id of the memento is the widget-id of the widget to load the data for.
     * </p>
     *
     * <p>
     * The Datetime widget does not store any metadata as the GUI has no specific state. The widget cannot be adjusten
     * in its visual representation.
     * </p>
     *
     * @param memento the memento pointing to the metadata part of this widget
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doLoadState(Memento)
     */
    @Override
    public void doLoadState(Memento memento)
    {
        // nothing to do here
    }

    /**
     *
     * Called to save the metadata like width, shown text format etc. of this widget.
     *
     * <p>
     * The handed in memento is exclusively created to store metadata for a specific widget managed by this controller.
     * The id of the memento is the widget-id of the widget to save the data for.
     * </p>
     *
     * <p>
     * The Datetime widget does not store any metadata as the GUI has no specific state. The widget cannot be adjusten
     * in its visual representation.
     * </p>
     *
     * @param memento the memento poiting to the metadata part of this widget
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doSaveState(Memento)
     */
    @Override
    public void doSaveState(Memento memento)
    {
        // nothing to do here
    }

    /**
     * Called to update the widget with the value from the associated Memento.
     *
     * <p>
     * Loads the attributes from the mementos of the model and stores them to the GUI element.
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doUpdateView(List)
     */
    @Override
    public void doUpdateView(List<String> changedMementos)
    {

        if (m_element.isDisposed())
        {
            return;
        }

        Memento memento = getWidgetMemento();
        int year = (int)memento.getInteger(memento.getAbsolutePath(MEMENTO_ATTRIBUTE_YEAR));
        int month = (int)memento.getInteger(memento.getAbsolutePath(MEMENTO_ATTRIBUTE_MONTH));
        int day = (int)memento.getInteger(memento.getAbsolutePath(MEMENTO_ATTRIBUTE_DAY));
        m_element.handleUpdateView(year, month, day);

    }

    public void doSetData(int year, int month, int day)
    {
        Memento rootMemento = getMemento();

        Memento memento = rootMemento.getMemento(m_mementoId);
        if (memento == null)
        {
            return;
        }

        internalSetData(memento, year, month, day);
    }

    /**
     * Called to updates the error/warnings markers from a {@link List} of {@link ValidationResult}s
     *
     * <p>
     * This widget cannot show warnings or errors.
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController#doUpdateValidationResults(List)
     */
    @Override
    public void doUpdateValidationResults(List<ValidationResult> results)
    {
        // Nothing to do here
    }

    /**
     * Get or create the memento of this widget.
     *
     * @return The memento
     */
    private Memento getWidgetMemento()
    {
        Memento rootMemento = getMemento();
        // obtain thr memento for this widget from the root memento
        Memento memento = rootMemento.getMemento(m_mementoId);
        // if the memento already exists, then return it
        if (memento != null)
        {
            return memento;
        }

        // else create a new memento for this widget
        memento = rootMemento.setMemento(m_mementoId);
        // set values to the current day in a transaction
        Calendar cal = Calendar.getInstance();
        cal.setTime(new Date());

        internalSetData(memento,
                        cal.get(Calendar.YEAR),
                        cal.get(Calendar.MONTH),
                        cal.get(Calendar.DAY_OF_MONTH));
        return memento;
    }

    /**
     * @param memento the memento on which to operate
     * @param year the year to set in the memento
     * @param month the month to set in the memento
     * @param day the day to set in the memento
     * @since 2010.a
     */
    private void internalSetData(Memento memento, int year, int month, int day)
    {
        // Get the MementoOperationHandler
        MementoOperationHandler mementoOperationHandler = getMementoOperationHandler();
        if (mementoOperationHandler == null)
        {
            return;
        }

        // begin transaction
        mementoOperationHandler.beginTransaction("SetData");

        // Set values
        mementoOperationHandler.setInteger(memento, MEMENTO_ATTRIBUTE_YEAR, year);
        mementoOperationHandler.setInteger(memento, MEMENTO_ATTRIBUTE_MONTH, month);
        mementoOperationHandler.setInteger(memento, MEMENTO_ATTRIBUTE_DAY, day);

        // commit transaction
        mementoOperationHandler.commitTransaction();
    }

} // MyDatetimeController

