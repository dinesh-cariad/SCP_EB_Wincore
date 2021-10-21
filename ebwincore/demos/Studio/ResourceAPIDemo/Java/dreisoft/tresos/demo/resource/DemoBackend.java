/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.demo.resource;

import dreisoft.tresos.demo.resource.examples.ConvertExample;
import dreisoft.tresos.demo.resource.examples.ExampleBase;
import dreisoft.tresos.demo.resource.examples.ImportECUConfigFromSysDExample;
import dreisoft.tresos.demo.resource.examples.MergeExample;
import dreisoft.tresos.demo.resource.generated.nls.ResourceAPIDemoNLS;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.api.gui.form.ISWTWidget;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTBrowser;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.ide.IDE;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;

/**
 * Controller part of the Resource API example runner wizard.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 11
 */
public class DemoBackend extends AbstractBackend
{

    // private fields

    /**
     * Mapping example names to their implementing instances (keeping the example order)
     */
    private LinkedHashMap<String, ExampleBase> m_examples = new LinkedHashMap<String, ExampleBase>();

    /**
     *
     * @since 11
     */
    public DemoBackend()
    {

        // instantiate the examples
        m_examples.put(ResourceAPIDemoNLS.NAME_CONVERT_EXAMPLE(), new ConvertExample(this));
        m_examples.put(ResourceAPIDemoNLS.NAME_MERGE_EXAMPLE(), new MergeExample(this));
        m_examples.put(ResourceAPIDemoNLS.NAME_IMPORT_SYSD_EXAMPLE(), new ImportECUConfigFromSysDExample(this));
    }

    /*
     * (non-Javadoc)
     *
     * @see dreisoft.tresos.guidedconfig.api.backend.AbstractBackend#doHandleUpdatedData(java.util.List)
     *
     * @since 11
     */
    @Override
    public void doHandleUpdatedData(List<String> affectedPaths)
    {

        boolean affected = false;
        String prefix = "/" + IConstants.MEMENTO_KEY_COMBO_EXAMPLE;
        for (String path : affectedPaths)
        {
            if (path.startsWith(prefix))
            {
                affected = true;
            }
        }

        if (affected)
        {
            updateExampleDescriptionText();
        }
    }

    /**
     * Updates the description text after a selection changes.
     *
     * @since 11
     */
    private void updateExampleDescriptionText()
    {
        String exampleNameSelected = getMemento().getString("/" + IConstants.MEMENTO_KEY_COMBO_EXAMPLE);

        String exampleDescription = getExampleDescription(exampleNameSelected);

        ISWTWidget widget = getPage().getWidget(IConstants.WIDGET_ID_EXAMPLE_DESCRIPTION);
        ((SWTBrowser)widget).setText(exampleDescription);

    }

    /**
     * Runs the selected example.
     *
     * @return list of {@link APIOperationStatus} as the result of the selected example
     */
    @Override
    public List<APIOperationStatus> doRun()
    {

        ArrayList<APIOperationStatus> result = new ArrayList<APIOperationStatus>();

        String exampleNameSelected = getMemento().getString("/" + IConstants.MEMENTO_KEY_COMBO_EXAMPLE);

        try
        {
            m_examples.get(exampleNameSelected).run(result);
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        return result;
    }

    /**
     * @return names of the available examples
     * @since 11
     */
    String[] getExampleNames()
    {
        ArrayList<String> names = new ArrayList<String>(m_examples.keySet());

        return names.toArray(new String[names.size()]);
    }

    /**
     * @param exampleName one of {@link #getExampleNames()}
     * @return the description text of the example (may contain <code>HTML</code> tags)
     * @since 11
     */
    String getExampleDescription(String exampleName)
    {
        return m_examples.get(exampleName).getDescription();
    }

    /**
     * Returns a file URI string within the project with the given relative path.
     *
     * @param relativePath the desired file relative to the project's folder
     * @return an URI string pointing to a file within the project
     * @since 11
     */
    public String getOutFileURI(String relativePath)
    {
        IProject project = getECUConfigProject();

        if (project == null)
        {
            return null;
        }

        IFile outfile = project.getFile(relativePath);

        String fileuriOut = outfile.getLocationURI().toASCIIString();

        return fileuriOut;
    }

    /**
     * Refreshes the resources of the ECU configuration project.
     *
     * @since 11
     */
    public void refreshECUConfigProject()
    {
        final IResource project = getECUConfigProject();

        if (project == null)
        {
            return;
        }

        Display.getDefault().asyncExec(new Runnable()
        {
            @Override
            public void run()
            {
                try
                {
                    project.refreshLocal(IResource.DEPTH_INFINITE, null);
                }
                catch (CoreException ex)
                {
                    ex.printStackTrace();
                }
            }
        });
    }

    /**
     * @return the ECU configuration project.
     *
     * @since 11
     */
    IProject getECUConfigProject()
    {
        String projectName = getECUConfigProjectName();

        if (projectName == null)
        {
            return null;
        }

        IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(projectName);

        return project;
    }

    /**
     * Open an editor for the given file (relative to the project).
     *
     * @since 11
     */
    public void openEditor(String relativePath)
    {
        final IProject project = getECUConfigProject();

        if (project == null)
        {
            return;
        }

        final String relPath = relativePath;

        Display.getDefault().asyncExec(new Runnable()
        {
            @Override
            public void run()
            {
                try
                {
                    IFile file = project.getFile(relPath);
                    IWorkbenchPage activePage = DemoPlugin.getDefault()
                                                          .getWorkbench()
                                                          .getActiveWorkbenchWindow()
                                                          .getActivePage();
                    IDE.openEditor(activePage, file);
                }
                catch (Exception ex)
                {
                    ex.printStackTrace();
                }
            }
        });

    }

    /**
     * @return the name of the ECU configuration project or null
     * @since 11
     */
    public String getECUConfigProjectName()
    {

        ECUConfigContext ecuConfigCtxt = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        if (ecuConfigCtxt == null)
        {
            return null;
        }
        String projectName = (String)ecuConfigCtxt.getAttribute(ECUConfigContext.ATTR_PROJECT_NAME);
        return projectName;
    }

}
