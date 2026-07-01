import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

const sidebars: SidebarsConfig = {
  tutorialSidebar: [
    {
      type: 'doc',
      id: 'index',
      label: 'Início',
    },
    {
      type: 'category',
      label: 'Planejamento e Requisitos',
      collapsed: false,
      items: [
        'eap',
        'requisitos',
        'normas',
        'analiseriscos',
      ],
    },
    {
      type: 'category',
      label: 'Documentação Técnica',
      collapsed: false,
      items: [
        'descricao-produto',
        'funcionamento',
        'reproducao-esp32',
        'pesquisa-bibliografica',
        'comparativo',
        'prototipo-site',
      ],
    },
  ],
};

export default sidebars;