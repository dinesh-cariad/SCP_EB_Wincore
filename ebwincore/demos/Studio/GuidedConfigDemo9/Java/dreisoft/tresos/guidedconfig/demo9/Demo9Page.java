/*
 * Copyright (C) EB Automotive
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TreeTableFlags;
import dreisoft.tresos.guidedconfig.api.gui.databinding.MementoOperationHandler;
import dreisoft.tresos.guidedconfig.api.gui.eventing.TreeTableSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.eventing.WidgetSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.form.ISelectionEventListener;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeTable;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo9.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.PartInitException;
import org.eclipse.ui.PlatformUI;

import java.util.ArrayList;

/**
 * This demo page shows a treetable which cannot be edited. It is only possible to add the toplevel elements of the
 * tree. Subitems for these elements can be added in the {@link TreePropertiesView}.
 */
public class Demo9Page extends AbstractPage
{

    //
    // defines

    /**
     * The Id of this page.
     */
    public static final String ID = "Demo9Page";

    /**
     * The TreeTable widget
     */
    private static SWTTreeTable m_tree;

    /**
     * The backend instance
     */
    private static AbstractBackend m_backend;

    /**
     * Memento of the last selected toplevel Row in the TreeTable
     */
    private static Memento m_oldSelection;

    public Demo9Page()
    {
        super();

    }

    /**
     * When closing the Guided configuration page, the properties view will be closed too.
     */
    @Override
    public void doHide()
    {
        super.doHide();
        TreePropertiesView propView = null;
        // hide PropertiesView
        propView = (TreePropertiesView)PlatformUI.getWorkbench()
                                                 .getActiveWorkbenchWindow()
                                                 .getActivePage()
                                                 .findView(TreePropertiesView.ID);

        if (propView != null)
        {
            PlatformUI.getWorkbench()
                      .getActiveWorkbenchWindow()
                      .getActivePage()
                      .hideView(propView);
        }

    }

    /*
     * (non-Javadoc)
     *
     * @see
     * dreisoft.tresos.guidedconfig.api.guidedconfig.BaseSinglePageContainer#createControls(org.eclipse.swt.widgets.
     * Composite)
     *
     * @since 2.1
     */
    @Override
    protected void doCreateControls(Composite parent)
    {
        // retrieve the widget factory and the backend
        WidgetFactory factory = getWidgetFactory();
        m_backend = factory.getBackend();

        // Group for the TreeTable widget
        SWTGroup outerGroup = factory.createGroup(parent,
                                                  "WidgetGroupdId_1",
                                                  WidgetFactory.GroupDecoration.TITLE,
                                                  WidgetFactory.GroupLayout.VERTICAL,
                                                  "My TreeTable");

        //
        // TreeTable
        SWTColumnDescription[] colDescs = new SWTColumnDescription[]{
            IConstants.TREE_COLUMN_DESCRIPTION,
            IConstants.TREE_COLUMN_DESCRIPTION_INT,
            IConstants.TREE_COLUMN_DESCRIPTION_STRING,
            IConstants.TREE_COLUMN_DESCRIPTION_BOOL,
            IConstants.TREE_COLUMN_DESCRIPTION_COMBO};

        // create the TreeTable
        m_tree = factory.createTreeTable(outerGroup,
                                         IConstants.WIDGET_ID_TREE,
                                         IConstants.MEMENTO_KEY_TREE,
                                         MyEclipseNLS.LABEL_TREE(),
                                         10,
                                         colDescs,
                                         new TreeTableFlags[]{
                                             TreeTableFlags.ACTION_ADD,
                                             TreeTableFlags.ACTION_REMOVE});

        // add a selection listener
        TreeSelectionListener treeEvent = new TreeSelectionListener();
        m_tree.getController().addSelectionEventListener(treeEvent);

        // layout the GUI
        factory.layout(parent);
    }

    /*
     * (non-Javadoc)
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage#getDescription()
     *
     * @since 2008.b
     */
    @Override
    public String doGetDescription()
    {
        return "Shows how to place guided configuration widgets outside of a guided config page";
    }

    /*
     * (non-Javadoc)
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage#getId()
     *
     * @since 2008.b
     */
    @Override
    public String doGetId()
    {
        return ID;
    }

    @Override
    public String doGetTitle()
    {
        return "Demo9Page";
    }

    /**
     * Copy the rows of the currently selected toplevel row of the TreeTable to the table memento which is shown on the
     * {@link TreePropertiesView}
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

    // ///////////////////////
    // Tree Selection changed listener
    // //////////////////////

    private static class TreeSelectionListener implements ISelectionEventListener
    {

        /**
         * If the selection of the toplevel rows changed, then the rowMemento is copied and shown in the
         * {@link TreePropertiesView} so the user can add rows.
         */
        @Override
        public void selectionChanged(WidgetSelectionEvent event)
        {
            if (!(event instanceof TreeTableSelectionEvent))
            {
                return;
            }
            TreeTableSelectionEvent treeEvent = (TreeTableSelectionEvent)event;

            Memento topLevelMemento = null;
            // Get the rootMemento of the page
            Memento rootMemento = m_backend.getMemento();
            // Get the tableMemento if it exists, else create it
            Memento tableMemento = rootMemento.getMemento(IConstants.MEMENTO_KEY_TABLE);
            if (tableMemento == null)
            {
                tableMemento = rootMemento.setMemento(IConstants.MEMENTO_KEY_TABLE);
            }

            // check if the selected row is a toplevel row of the TreeTable
            if (treeEvent.getClickedRowMemento()
                         .getParent()
                         .getPath()
                         .equals(m_tree.getController().getWidgetMementoPath()))
            {
                // toplevel selected
                topLevelMemento = treeEvent.getClickedRowMemento();
            }
            else
            {
                // level 2 selected -> do not change the table inside the TreePropertiesView
                return;
            }
            // check if selection really changed
            if (topLevelMemento == m_oldSelection)
            {
                return;
            }

            // remember previous selection
            m_oldSelection = topLevelMemento;
            // copy the rows of the currently selected toplevel treeMemento to the table
            copyRows(m_backend, topLevelMemento, tableMemento);

            // open/show the TreePropertiesView
            TreePropertiesView propView = null;
            // open PropertiesView
            try
            {
                propView = (TreePropertiesView)PlatformUI.getWorkbench()
                                                         .getActiveWorkbenchWindow()
                                                         .getActivePage()
                                                         .showView(TreePropertiesView.ID);
                propView.initViewer(m_backend);
                propView.refreshViewer(tableMemento, topLevelMemento);
            }
            catch (PartInitException ex)
            {
                ex.printStackTrace();
            }
        }
    }
}
