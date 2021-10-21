/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo3;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.demo3.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * Single Page wizard for the autoconfiguration dialog.
 *
 * <p>
 * The backend implementation of this wizard can be found in {@link Demo3Backend}.
 * </p>
 *
 * <p>
 * This demo implementation shows a checkbox which controls if the change of the project configuration should take
 * place.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo3Page extends AbstractPage
{

    /**
     * Page-Id.
     */
    public static final String ID = "MyAutoConfigurePage";

    /**
     * WidgetId of the collapseable group.
     */
    public static final String WIDGETID_GROUP = "GroupWidget1";

    /**
     * WidgetId of the browser.
     */
    public static final String WIDGETID_BROWSER = "Browser1";
    /**
     * MementoId of the browser.
     */
    public static final String MEMENTOID_BROWSER = "Browser1";

    /**
     * WidgetId of the label.
     */
    public static final String WIDGETID_LABEL = "Label1";

    /**
     * WidgetId of the checkbox
     */
    public static final String WIDGETID_CHECKBOX = "CheckboxChangedValuesWidget";

    /**
     * MementoId of the checkbox, to store if the wizard should change the model.
     */
    public static final String MEMENTOID_CHECKBOX = "CheckboxChangedValuesMemento";

    @Override
    public String doGetId()
    {
        return ID;
    }

    @Override
    public String doGetTitle()
    {
        return MyEclipseNLS.TITLE_AUTOCONFIGURE();
    }

    @Override
    public String doGetDescription()
    {
        return MyEclipseNLS.TOOLTIP_AUTOCONFIGURE();
    }

    /**
     * Render the GUI.
     *
     * <p>
     * This demo implementation shows a group that can be collapsed with a title, a browser and a checkbox which
     * controls if the wizard changes the configuration or not.
     * </p>
     *
     * @param parent The composite to place the GUI in
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage#doCreateControls(Composite)
     */
    @Override
    protected void doCreateControls(Composite parent)
    {
        // Get the factory via which all widgets get created
        WidgetFactory factory = getWidgetFactory();

        // Create a group that can be collapsed and expanded
        SWTGroup group = factory.createGroup(parent,
                                             WIDGETID_GROUP,
                                             WidgetFactory.GroupDecoration.LINE,
                                             WidgetFactory.GroupLayout.VERTICAL,
                                             MyEclipseNLS.TITLE_GROUP_RESET_VALUES());

        // Create a browser with an explanation what will be done using this wizard
        StringBuffer browserText = new StringBuffer();
        browserText.append("<body style='background-color:#CCFFFF'>");
        browserText.append("The values on tab page 'Stuff' are changed to:<br/>");
        browserText.append("<ul><li>Float Activator: false</li>");
        browserText.append("<li>Labeled Int: 23</li>");
        browserText.append("<li>String: no value</li>");
        browserText.append("<li>Float: 23.5</li></ul>");
        browserText.append("</body>");
        factory.createBrowser(group,
                              WIDGETID_BROWSER,
                              MEMENTOID_BROWSER,
                              browserText.toString(),
                              4,
                              -1,
                              -1,
                              100,
                              true);

        // Create a checkbox with a label in front
        factory.createLabel(group, WIDGETID_LABEL, WIDGETID_LABEL, MyEclipseNLS.LABEL_CHECKBOX_RESET_VALUES());
        factory.createCheckBox(group, WIDGETID_CHECKBOX, MEMENTOID_CHECKBOX);

        // Layout the GUI
        factory.layout(parent);
    }

} // Demo3Page

