/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.ButtonFlags;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TableFlags;
import dreisoft.tresos.guidedconfig.api.gui.databinding.MementoOperationHandler;
import dreisoft.tresos.guidedconfig.api.gui.eventing.ButtonSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.eventing.WidgetSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.form.ISelectionEventListener;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTButton;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTable;
import dreisoft.tresos.guidedconfig.api.memento.Memento;

import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

import java.util.ArrayList;

/**
 * A view part using Guided Config widgets.
 *
 * @since 11
 */
public class TreePropertiesView extends ViewPart
{

    /**
     * the eclipse-plugin-registry-id of this view
     */
    public static final String ID = "dreisoft.tresos.guidedconfig.demo9.TreePropertiesView";

    /**
     * The parent composite
     */
    private static Composite m_parent;
    /**
     * The table to show the subitems of the treeTable
     */
    private static SWTTable m_table;
    /**
     * The backend class of the guided configuration page
     */
    private static AbstractBackend m_backend;
    /**
     * Memento of the last selected toplevel Row in the TreeTable of the guided configuration page
     */
    private static Memento m_selectedTreeItem;

    /**
     * @since 11
     */
    public TreePropertiesView()
    {
        super();
    }

    @Override
    public void createPartControl(Composite parent)
    {
        m_parent = parent;
        // set layout
        GridLayout layout = new GridLayout(1, true);
        layout.marginHeight = 0;
        layout.marginWidth = 0;
        layout.horizontalSpacing = 0;
        layout.verticalSpacing = 0;
        m_parent.setLayout(layout);

    }

    public void initViewer(AbstractBackend backend)
    {
        if ((m_table != null) && !m_table.isDisposed())
        {
            return;
        }
        m_backend = backend;
        WidgetFactory factory = backend.getWidgetFactory();

        SWTColumnDescription[] colDescs = new SWTColumnDescription[]{
            IConstants.TREE_COLUMN_DESCRIPTION_INT,
            IConstants.TREE_COLUMN_DESCRIPTION_STRING,
            IConstants.TREE_COLUMN_DESCRIPTION_BOOL,
            IConstants.TREE_COLUMN_DESCRIPTION_COMBO};

        m_table = factory.createTable(m_parent,
                                      IConstants.WIDGET_ID_TABLE,
                                      IConstants.MEMENTO_KEY_TABLE,
                                      "My Table",
                                      10,
                                      colDescs,
                                      new TableFlags[]{
                                          TableFlags.ACTION_ADD,
                                          TableFlags.ACTION_REMOVE,
                                          TableFlags.ACTION_DUPLICATE});

        TableMouseListener tableEvent = new TableMouseListener();
        m_table.addMouseListener(tableEvent);

        // commit changes button
        SWTButton commitChangesButton = factory.createButton(m_parent,
                                                             "CommitChanges",
                                                             "CommitChanges",
                                                             "Commit changes",
                                                             null,
                                                             ButtonFlags.PUSH);
        // add event listener for the button
        CommitChangesSelection commit = new CommitChangesSelection();
        commitChangesButton.getController().addSelectionEventListener(commit);

        factory.layout(m_parent);
        m_parent.layout(true, true);

    }

    public void refreshViewer(Memento tableRoot, Memento selectedTreeItem)
    {
        m_selectedTreeItem = selectedTreeItem;
        m_backend.updateView(tableRoot.getPath());
        m_backend.validate();
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.ui.part.WorkbenchPart#setFocus()
     *
     * @since 11
     */
    @Override
    public void setFocus()
    {
    }

    /**
     * Copy the rows of the Table to the currently selected toplevel row of the TreeTable
     *
     * @param backend The backend class
     * @param sourceMemento The source memento from which to copy the values
     * @param destinationMemento The destination memento
     * @since 11
     */
    private static void copyRows(AbstractBackend backend, Memento sourceMemento, Memento destinationMemento)
    {
        if (destinationMemento != null)
        {
            MementoOperationHandler moh = backend.getMementoOperationHandler();
            ArrayList<String> childPaths = new ArrayList<String>();
            for (Memento child : moh.getRows(destinationMemento))
            {
                childPaths.add(child.getPath());
            }
            moh.unsetMementos(childPaths.toArray(new String[0]));
        }
        for (Memento child : sourceMemento.getChildren())
        {
            destinationMemento.setMemento(child.clone());
        }
    }

    private class TableMouseListener implements MouseListener
    {

        @Override
        public void mouseDoubleClick(MouseEvent e)
        {
            // when double clicking on a table entry, open the external dialog
            // and let the user change the values
            int[] rows = m_table.getSelections();
            if (rows.length != 1)
            {
                return;
            }
            int clickedRow = rows[0];

            ExternDialog dialog = new ExternDialog(
                m_parent.getShell(),
                m_backend,
                m_table.getController().getRowMemento(clickedRow));

            dialog.open();
        }

        @Override
        public void mouseDown(MouseEvent e)
        {
        }

        @Override
        public void mouseUp(MouseEvent e)
        {
        }
    }

    // ///////////////////////
    // Button Selection listener
    // //////////////////////
    private static class CommitChangesSelection implements ISelectionEventListener
    {

        /**
         * Copy back the changed rows from the table of the {@link TreePropertiesView} to the treeTable memento and
         * validate/refresh the GUI
         */
        @Override
        public void selectionChanged(WidgetSelectionEvent event)
        {
            if (!(event instanceof ButtonSelectionEvent))
            {
                return;
            }
            if (m_selectedTreeItem != null)
            {
                Memento rootMemento = m_backend.getMemento();
                Memento tableMemento = rootMemento.getMemento(IConstants.MEMENTO_KEY_TABLE);
                // commit the changes
                copyRows(m_backend, tableMemento, m_selectedTreeItem);
                // update the TreeTable
                m_backend.updateView(rootMemento.getMemento(IConstants.MEMENTO_KEY_TREE).getPath());
                // validate and update validation results in the GUI
                m_backend.validate();
            }
        }
    }

}
