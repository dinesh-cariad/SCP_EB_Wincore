/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo3;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractResultWidget;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ControlEvent;
import org.eclipse.swt.events.ControlListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import java.util.List;

/**
 * Example ResultWidget, where information can be presented after wizard has run in addition to logged errors.
 *
 * <p>
 * This demo shows changed nodes in an Eclipse table viewer.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo3ResultWidget extends AbstractResultWidget implements IStructuredContentProvider,
        ITableLabelProvider
{

    //
    // private members

    /**
     * List of changed nodes as gathered by the {@link Demo3Backend} in a push event.
     */
    private List<DCtxt> m_changedNodes = null;

    /**
     * TableViewer to present the changed values to the user
     */
    private TableViewer m_tableViewer;

    /**
     * Default constructor.
     */
    public Demo3ResultWidget()
    {
    }

    /**
     * Initialize the result widget.
     *
     * <p>
     * This method is called after instantiation of the widget.
     * </p>
     *
     * <p>
     * This demo implementation queries the list of changed nodes from the backend.
     * </p>
     *
     * @param statusList The list of logged errors (need not be processed)
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractResultWidget#doInit(List)
     */
    @Override
    public void doInit(List<APIOperationStatus> statusList)
    {
        super.doInit(statusList);

        if (getBackend() instanceof Demo3Backend)
        {
            // Used by Demo3Backend
            m_changedNodes = ((Demo3Backend)getBackend()).getChangedNodes();
        }
    }

    /**
     * Create the GUI.
     *
     * <p>
     * This implementation creates an Eclipse TableViewer and shows the changed nodes.
     * </p>
     *
     * @param parent The parent composite
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractResultWidget#doCreateContent(Composite)
     */
    @Override
    public void doCreateContent(Composite parent)
    {
        // Create an Eclipse Table Viewer
        m_tableViewer = new TableViewer(parent);
        m_tableViewer.getTable().setHeaderVisible(true);
        m_tableViewer.getTable().setLinesVisible(true);
        m_tableViewer.setContentProvider(this);
        m_tableViewer.setLabelProvider(this);
        m_tableViewer.setColumnProperties(new String[]{"Executed Operations"});
        final TableColumn colName = new TableColumn(m_tableViewer.getTable(), SWT.LEFT);
        colName.setText("Changed node");
        final TableColumn colValue = new TableColumn(m_tableViewer.getTable(), SWT.LEFT);
        colValue.setText("New Value");
        colValue.setWidth(100);
        m_tableViewer.getTable().addControlListener(new ControlListener()
        {

            @Override
            public void controlMoved(ControlEvent e)
            {
            }

            @Override
            public void controlResized(ControlEvent e)
            {
                if (m_tableViewer == null)
                {
                    return;
                }
                Table t = m_tableViewer.getTable();
                colName.setWidth(t.getSize().x - (2 * t.getBorderWidth()) - colValue.getWidth());
            }

        });
        m_tableViewer.setInput(m_changedNodes);
    }

    //
    // Table Viewer content and label provider

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.IStructuredContentProvider#getElements(java.lang.Object)
     */
    @Override
    public Object[] getElements(Object inputElement)
    {
        return m_changedNodes == null ? new Object[0] : m_changedNodes.toArray();
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.IContentProvider#inputChanged(org.eclipse.jface.viewers.Viewer, java.lang.Object,
     * java.lang.Object)
     */
    @Override
    public void inputChanged(Viewer viewer, Object oldInput, Object newInput)
    {
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.ITableLabelProvider#getColumnImage(java.lang.Object, int)
     */
    @Override
    public Image getColumnImage(Object element, int columnIndex)
    {
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.ITableLabelProvider#getColumnText(java.lang.Object, int)
     */
    @Override
    public String getColumnText(Object element, int columnIndex)
    {
        if (columnIndex == 0)
        {
            return ((DCtxt)element).getPath();
        }
        if (columnIndex == 1)
        {
            return ((DCtxt)element).var.getValue();
        }
        return null;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.IBaseLabelProvider#addListener(org.eclipse.jface.viewers.ILabelProviderListener)
     */
    @Override
    public void addListener(ILabelProviderListener listener)
    {
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.jface.viewers.IBaseLabelProvider#isLabelProperty(java.lang.Object, java.lang.String)
     */
    @Override
    public boolean isLabelProperty(Object element, String property)
    {
        return false;
    }

    /*
     * (non-Javadoc)
     *
     * @see
     * org.eclipse.jface.viewers.IBaseLabelProvider#removeListener(org.eclipse.jface.viewers.ILabelProviderListener)
     */
    @Override
    public void removeListener(ILabelProviderListener listener)
    {
    }

} // MyResultWidget

