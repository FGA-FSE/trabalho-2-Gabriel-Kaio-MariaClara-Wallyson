import type {Config} from '@docusaurus/types';
import {themes} from 'prism-react-renderer';
import github from 'prism-react-renderer/themes/github';
import dracula from 'prism-react-renderer/themes/dracula';

const config: Config = {
  title: 'Trabalho 2',
  tagline: 'Documentação do projeto',
  favicon: 'img/favicon.ico',

  url: 'https://fga-fse.github.io',
  baseUrl: '/trabalho-2-Gabriel-Kaio-MariaClara-Wallyson/',

  organizationName: 'FGA-FSE',
  projectName: 'trabalho-2-Gabriel-Kaio-MariaClara-Wallyson',

  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  i18n: {
    defaultLocale: 'pt-BR',
    locales: ['pt-BR'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          routeBasePath: '/',
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      },
    ],
  ],

  themes: ['@docusaurus/theme-mermaid'],
  markdown: {
    mermaid: true,
  },

  themeConfig: {
    image: 'img/docusaurus-social-card.jpg',

    navbar: {
      title: 'Controlador de Irrigação Inteligente',
      logo: {
        alt: 'Logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          type: 'doc',
          docId: 'index',
          label: 'Documentação',
          position: 'left',
        },
        {
          href: 'https://github.com/FGA-FSE/trabalho-2-Gabriel-Kaio-MariaClara-Wallyson',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },

    footer: {
      style: 'dark',
      copyright: `© ${new Date().getFullYear()} Trabalho 2 - FGA`,
    },

    prism: {
      theme: themes.github,
      darkTheme: themes.dracula,
    },
  },
};

export default config;
