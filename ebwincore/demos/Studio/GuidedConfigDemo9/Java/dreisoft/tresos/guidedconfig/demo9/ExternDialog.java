/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TextFormat;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo9.generated.nls.MyEclipseNLS;

import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;

/**
 * This dialog shows the parameters for the selected row of the {@link TreePropertiesView}, so they can be changed.
 */
public class ExternDialog extends TitleAreaDialog
{

    /**
     * The backend instance
     */
    private AbstractBackend m_backend = null;
    /**
     * The memento of the currently selected row
     */
    private Memento m_selectedRow = null;

    /**
     * The constructor of the extern dialog
     *
     * @param parentShell the parent shell for the dialog
     * @param backend the backend class
     * @param rowMemento the memento of the currently selected row in the {@link TreePropertiesView}.
     */
    public ExternDialog(Shell parentShell, AbstractBackend backend, Memento rowMemento)
    {
        super(parentShell);
        setShellStyle(getShellStyle() | SWT.RESIZE | SWT.MODELESS);
        m_backend = backend;
        m_selectedRow = rowMemento;
        setHelpAvailable(false);
    }

    /**
     * Create only one OK button for the dialog. As the values are stored directly in the memento and shown in the
     * {@link TreePropertiesView} there is nothing to be canceled.
     */
    @Override
    protected void createButtonsForButtonBar(Composite parent)
    {
        createButton(parent, IDialogConstants.OK_ID, IDialogConstants.OK_LABEL, true);
    }

    @Override
    protected Control createDialogArea(Composite parent)
    {
        Composite container = (Composite)super.createDialogArea(parent);
        setTitle("Adapt values");
        setMessage("Please adapt the values.");

        // get the widget factory via which all widgets are managed and created
        WidgetFactory factory = m_backend.getWidgetFactory();

        // set layout
        GridLayout layout = new GridLayout(1, true);
        layout.marginHeight = 0;
        layout.marginWidth = 0;
        layout.horizontalSpacing = 0;
        layout.verticalSpacing = 0;
        container.setLayout(layout);

        // create common group
        SWTGroup outerGroup = factory.createGroup(container,
                                                  "WidgetGroupdId_1",
                                                  WidgetFactory.GroupDecoration.TITLE,
                                                  WidgetFactory.GroupLayout.VERTICAL,
                                                  "Values of the currently selected table row:");

        // Integer
        factory.createLabel(outerGroup,
                            IConstants.WIDGET_ID_LABEL + "Int",
                            IConstants.MEMENTO_KEY_LABEL + "Int",
                            MyEclipseNLS.LABEL_COLUMN_INT());

        factory.createText(outerGroup, MyEclipseNLS.LABEL_COLUMN_INT(), m_selectedRow.getPath()
            + "/"
            + IConstants.MEMENTO_COLUMN_INT, TextFormat.INTEGER);

        // String
        factory.createLabel(outerGroup, IConstants.WIDGET_ID_LABEL + "String", IConstants.MEMENTO_KEY_LABEL
            + "String", MyEclipseNLS.LABEL_COLUMN_STRING());

        factory.createText(outerGroup, MyEclipseNLS.LABEL_COLUMN_STRING(), m_selectedRow.getPath()
            + "/"
            + IConstants.MEMENTO_COLUMN_STRING, TextFormat.TEXT);

        // Bool
        factory.createLabel(outerGroup,
                            IConstants.WIDGET_ID_LABEL + "Bool",
                            IConstants.MEMENTO_KEY_LABEL + "Bool",
                            MyEclipseNLS.LABEL_COLUMN_BOOL());

        factory.createCheckBox(outerGroup, MyEclipseNLS.LABEL_COLUMN_BOOL(), m_selectedRow.getPath()
            + "/"
            + IConstants.MEMENTO_COLUMN_BOOL);

        factory.createSpacer(outerGroup, 2, 1);
        // Combo
        factory.createLabel(outerGroup,
                            IConstants.WIDGET_ID_LABEL + "Combo",
                            IConstants.MEMENTO_KEY_LABEL + "Combo",
                            MyEclipseNLS.LABEL_COLUMN_COMBO());

        factory.createCombo(outerGroup, MyEclipseNLS.LABEL_COLUMN_COMBO(), m_selectedRow.getPath()
            + "/"
            + IConstants.MEMENTO_COLUMN_COMBO, TextFormat.TEXT, new String[]{"ONE", "TWO", "THREE"});

        // update view and validation results
        factory.layout(container);
        m_backend.updateView(m_selectedRow.getPath());
        m_backend.validate();
        return container;
    }
}
