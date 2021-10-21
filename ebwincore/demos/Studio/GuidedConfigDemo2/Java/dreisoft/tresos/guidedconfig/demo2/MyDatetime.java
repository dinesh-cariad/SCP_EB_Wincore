/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo2;

import dreisoft.tresos.guidedconfig.api.gui.form.AbstractWidgetController;
import dreisoft.tresos.guidedconfig.api.gui.form.ISWTWidget;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTWidgetLayoutData;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.DateTime;

/**
 * This is a sample for a custom widget managed by the wizard.
 *
 * <p>
 * The MyDatetime widget is used by the {@link Demo2Page}.
 * </p>
 *
 * <p>
 * The custom widget is composed of two classes MyDatetime and {@link MyDatetimeController}. This class is only
 * responsible for the GUI representation of the widget. The control over the data flow is done by the
 * {@link MyDatetimeController} class to seperate View and Controller.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class MyDatetime implements ISWTWidget
{

    //
    // private members

    /**
     * The widgetId of this widget.
     */
    private String m_widgetId;

    /**
     * The parent composite.
     */
    private Composite m_parent;

    /**
     * The actual GUI widget.
     */
    private DateTime m_entry = null;

    /**
     * The controller for this widget.
     */
    private MyDatetimeController m_controller = null;

    /**
     * Constructor for the Datetime widget.
     *
     * @param widgetId The widgetId of the Datetime widget
     * @param parent The parent composite onto which to place the widget
     */
    public MyDatetime(String widgetId, Composite parent)
    {
        m_widgetId = widgetId;
        m_parent = parent;

        // create the DateTime widget with calender type
        m_entry = new DateTime(parent, SWT.CALENDAR);

        // set the Layout of the widget
        SWTWidgetLayoutData gcld = new SWTWidgetLayoutData();
        // In a vertically laid out group one row consists of 4 cells of GridData
        gcld.setVerticalGridData(new GridData(SWT.BEGINNING, SWT.BEGINNING, true, false, 4, 1));
        // In a horizontally laid out group all widgets should have a width of 1 cell
        gcld.setHorizontalGridData(new GridData(SWT.BEGINNING, SWT.CENTER, false, true, 1, 1));
        m_entry.setLayoutData(gcld);

        // add selection Listener, to get informed about value changes
        m_entry.addSelectionListener(new SelectionAdapter()
        {

            @Override
            public void widgetSelected(SelectionEvent e)
            {
                if (m_controller != null)
                {
                    m_controller.doSetData(m_entry.getYear(), m_entry.getMonth(), m_entry.getDay());
                    // Send data to the controller which dispatches changes to the mementos
                    m_controller.doUpdateData();
                }
            }
        });

        // Add dispose listener for notifying the controller about the widget being disposed.
        // Required when widget is used outside of a guided configuration Page.
        m_entry.addDisposeListener(new DisposeListener()
        {
            @Override
            public void widgetDisposed(DisposeEvent e)
            {
                if (m_controller != null)
                {
                    m_controller.doWidgetIsDisposed();
                }
            }
        });

    }

    /**
     * Query the widgetId of the widget.
     *
     * @return The widgetID (not null)
     * @see dreisoft.tresos.guidedconfig.api.gui.form.ISWTWidget#getWidgetId()
     */
    @Override
    public String getWidgetId()
    {
        return m_widgetId;
    }

    /**
     * The parent composite of the widget.
     *
     * @return The parent gui element
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.form.ISWTWidget#getParent()
     */
    @Override
    public Composite getParent()
    {
        return m_parent;
    }

    /**
     * Set the {@link AbstractWidgetController} for this widget for implementing an Model-View-Controller pattern with
     * an extra class controlling this widget.
     *
     * <p>
     * Called by the controller.
     * </p>
     *
     * @param controller The {@link AbstractWidgetController}
     */
    public void setController(AbstractWidgetController controller)
    {
        if (!(controller instanceof MyDatetimeController))
        {
            throw new IllegalStateException("Wrong controller");
        }
        m_controller = (MyDatetimeController)controller;
    }

    /**
     * Sets the focus to the Datetime widget.
     *
     * <p>
     * Used for navigation.
     * </p>
     *
     * <p>
     * BEWARE: Navigation support is not yet fully implemented for custom widgets. New APIs will follow.
     * </p>
     *
     * @return true if the control got focus, and false if it was unable to.
     */
    public boolean setFocus()
    {
        return getEntry().setFocus();
    }

    /**
     * Changes the enablement state of the widget.
     *
     * <p>
     * Called by the controller.
     * </p>
     *
     * @param enabled true to set it enabled, false otherwise
     */
    public void setEnabled(boolean enabled)
    {
        getEntry().setEnabled(enabled);
    }

    /**
     * Retrieve the enablement state of the widget.
     *
     * @return true if it is enabled, false if not
     */
    public boolean isEnabled()
    {
        return getEntry().isEnabled();
    }

    /**
     * Retrieve the base widget
     *
     * @return the {@link DateTime} widget
     */
    private DateTime getEntry()
    {
        return m_entry;
    }

    public boolean isDisposed()
    {
        return m_entry.isDisposed();
    }

    /**
     * Update the GUI widget.
     *
     * <p>
     * Called by the controller.
     * </p>
     */
    public void handleUpdateView(int year, int month, int day)
    {
        m_entry.setYear(year);
        m_entry.setMonth(month);
        m_entry.setDay(day);
        m_entry.update();
    }

} // MyDatetime

